#include "ccameradevice.h"
#include "comdef.h"
#include <QString>
#include <QDateTime>
#include "comfunction.h"

void rotate_copy_yuv(uint8_t* src, uint8_t* dest, int w, int h, int front)
{
    memcpy(dest, src, w * h);
}

uint8_t* nv21to420p(uint8_t* data, int w, int h, int front){
    int size = w * h;
    uint8_t *dt = (uint8_t *)malloc(size * 3 / 2);

    //由于camera采集的yuv数据前置、后置都有旋转角度，
    //直接保存的话，图片是旋转了的，
    //此处同时做了旋转和复制
    rotate_copy_yuv(data, dt, w, h, front);

    uint8_t *temp = (uint8_t *)malloc(size / 4);
    uint8_t *p = temp;

    for(int i = size + 1; i < size * 3 / 2; i = i + 2, p = p + 1){
        *p = *(data + i);
    }

    //同上
    rotate_copy_yuv(temp, dt + size, w / 2, h / 2, front);
    p = temp;

    for(int i = size; i < size * 3 / 2; i = i + 2, p = p + 1){
        *p = *(data + i);
    }

    //同上
    rotate_copy_yuv(temp, dt + size * 5 / 4 , w / 2, h / 2, front);
    free(temp);

    return dt;
}



CCameraDevice::CCameraDevice(QObject *parent) : QThread(parent)
{
    m_stop = false;
    m_camfd = 0;
    m_buf_num = 1;
    m_usr_buf = NULL;
    m_yuv_buf = NULL;
    m_screenshot = false;
    m_userData = NULL;

    // SDL
    if(SDL_Init(SDL_INIT_EVERYTHING))
    {
        qInfo("SDL init err");
        return;
    }
    av_register_all();
    m_window = nullptr;
    m_render = nullptr;
    m_texture= nullptr;
    m_thread = nullptr;
}

CCameraDevice::~CCameraDevice()
{
    Close();
}

bool CCameraDevice::Open(VideoParam param, pf_AVDataCallBack callBack,void* userData)
{
    m_userData = userData;
    Close();
    m_screenshot = false;
    if(param.devPath.empty() || param.height <=0 || param.width <= 0)
    {
        LOG(QString("param error"));
        return false;
    }
    struct v4l2_format format;
    memset(&format, 0, sizeof(format));
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = param.videoCaptureFmt;
    format.fmt.pix.width = param.width;
    format.fmt.pix.height = param.height;

    m_param = param;
    m_camfd = open(param.devPath.c_str(), O_RDWR);
    if(m_camfd < 0)
    {
        LOG(QString("open device fail, %1,err=%2").arg(param.devPath.c_str()).arg(strerror(errno)) );
        return false;
    }
    int index = 0;
    if(0 != ioctl(m_camfd, VIDIOC_S_INPUT, &index))
    {
        LOG(QString("ioctl VIDIOC_S_INPUT fail,err=%1").arg(strerror(errno)));
        close(m_camfd);
        return false;
    }
    int ret = ioctl(m_camfd, VIDIOC_TRY_FMT, &format);
    if (ret != 0)
    {
        LOG(QString("ioctl VIDIOC_TRY_FMT fail,err=%1").arg(strerror(errno)));
        close(m_camfd);
        return false;
    }
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(m_camfd, VIDIOC_S_FMT, &format);
    if (ret != 0)
    {
        LOG(QString("ioctl VIDIOC_S_FMT fail,err=%1").arg(strerror(errno)));
        close(m_camfd);
        return false;
    }
    m_stop = false;
    m_avCallBack = callBack;

    if(m_window == nullptr)
    {
        m_window = SDL_CreateWindowFrom((void*)param.renderWidget->winId());
    }
    if(m_window == nullptr)
        return false;

    if(m_render == nullptr)
    {
        m_render = SDL_CreateRenderer(m_window,-1,SDL_RENDERER_SOFTWARE);
    }
    if(m_render == nullptr)
    {
        qInfo("SDL_CreateRender error=%s",SDL_GetError());
        return false;
    }
    if(m_texture == nullptr)
    {
        m_texture = SDL_CreateTexture(m_render,SDL_PIXELFORMAT_IYUV,SDL_TEXTUREACCESS_STREAMING
                                      ,param.width,param.height);
    }
    start();
    return true;
}

void CCameraDevice::Close()
{
    m_stop = true;
    wait();
    if(m_camfd <= 0)
        return;
    int buffer_type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ioctl(m_camfd, VIDIOC_STREAMOFF, &buffer_type);

    if(m_render)
    {
        SDL_DestroyRenderer(m_render);
        m_render = nullptr;
    }
    if(m_texture)
    {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
    }
    close(m_camfd);
    m_camfd = 0;
}

void CCameraDevice::ScreensShot()
{
    m_screenshot = true;
}
int CCameraDevice::InitMMap()
{
    if(m_camfd <=0 || m_buf_num < 1)
    {
        return -1;
    }
    m_usr_buf = (BUFTYPE*)calloc(m_buf_num, sizeof(BUFTYPE));
    if (!m_usr_buf)
    {
        LOG("calloc  fail");
        return -2;
    }
    m_yuv_buf = (BUFTYPE*)calloc(1, sizeof(BUFTYPE));
    if (!m_yuv_buf)
    {
        LOG("calloc  fail");
        return -2;
    }
    m_bmpBuff.start = (void*)calloc(m_param.width*m_param.height*3,sizeof(char*));
    m_bmpBuff.length = m_param.width*m_param.height*3;
    int yuv_len = m_param.width*m_param.height*3/2;
    m_yuv_buf->start = (void*)calloc(yuv_len,sizeof(char*));
    m_yuv_buf->length = yuv_len;
    struct v4l2_requestbuffers req;
    req.count = m_buf_num;                    //帧缓冲数量
    req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE; //视频捕获缓冲区类型
    req.memory = V4L2_MEMORY_MMAP;          //内存映射方式
    if (ioctl(m_camfd, VIDIOC_REQBUFS, &req) < 0)
    {
        LOG("VIDIOC_REQBUFS fail");
        return -3;
    }

    for(unsigned int i = 0; i < m_buf_num; ++i)
    {
        struct v4l2_buffer v4l2_buf;
        memset(&v4l2_buf, 0, sizeof(v4l2_buf));
        v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        v4l2_buf.memory = V4L2_MEMORY_MMAP;
        v4l2_buf.index = i;
        if(ioctl(m_camfd , VIDIOC_QUERYBUF, &v4l2_buf) < 0)
        {
            LOG("VIDIOC_QUERYBUF failed\n");
            return -4;
        }
        m_usr_buf[i].length = v4l2_buf.length;
        m_usr_buf[i].start = (char *)mmap(0, v4l2_buf.length, PROT_READ | PROT_WRITE, MAP_SHARED, m_camfd, v4l2_buf.m.offset);

        if (MAP_FAILED == m_usr_buf[i].start)
        {
            LOG("mmap failed");
            return -5;
        }else
        {
            if (ioctl(m_camfd, VIDIOC_QBUF, &v4l2_buf) < 0)
            {
                LOG("VIDIOC_QBUF failed");
                return -6;
            }
        }
    }
    return 0;
}
int CCameraDevice::StreamOn()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(m_camfd, VIDIOC_STREAMON, &type) < 0)
    {
        LOG("VIDIOC_STREAMON failed");
        return -1;
    }
    return 0;
}

int CCameraDevice::WriteFrame()
{
    struct v4l2_buffer v4l2_buf;
    v4l2_buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    v4l2_buf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(m_camfd, VIDIOC_DQBUF, &v4l2_buf) < 0) // 内核缓冲区出队列
    {
        LOG("VIDIOC_DQBUF failed, dropped frame\n");
        return -1;
    }

    //预览
    bool bYUV420 = Convert2YUV420(&m_usr_buf[v4l2_buf.index],m_param,&m_yuv_buf[0]);
    if(m_param.renderWidget && bYUV420)
    {
        int videoWidth = m_param.width;
        int videoHeight = m_param.height;
        int winWidth = m_param.renderWidget->width();
        int winHeight = m_param.renderWidget->height();


        SDL_Rect rect;
        rect.x = 0;
        rect.y = 0;
        rect.h = winHeight;
        rect.w = winWidth;

        SDL_SetWindowSize(m_window,rect.w ,rect.h);
        SDL_UpdateTexture(m_texture,NULL,m_yuv_buf[0].start,m_param.width);
        SDL_RenderClear(m_render);
        SDL_RenderCopy(m_render,m_texture,NULL,&rect);
        SDL_RenderPresent(m_render);
    }
    if(bYUV420 && m_avCallBack)
    {
        m_avCallBack((char*)m_yuv_buf[0].start,m_yuv_buf[0].length,&m_param,NULL,m_userData);
    }
    //截图
    if(m_screenshot)
    {

        QString fileName = QString("%1.jpg").arg(QDateTime::currentDateTime().toString("yyyy_dd_MM_hh_mm_ss_zzz"));
        char buffer[256];
        sprintf(buffer,"%s%s",m_param.saveDir.c_str(),fileName.toStdString().c_str());
        SaveYUVToJPG(&m_yuv_buf[0],m_param,buffer);
        m_screenshot = false;
    }
    if (ioctl(m_camfd, VIDIOC_QBUF, &v4l2_buf) < 0)
    {
        LOG("VIDIOC_QBUF failed, dropped frame\n");
        return -2;
    }
    return v4l2_buf.index;
}

bool CCameraDevice::Convert2YUV420(BUFTYPE *srcBuff, VideoParam param, BUFTYPE *dtsBuff)
{
    if(srcBuff == NULL || srcBuff->start == NULL || dtsBuff == NULL || dtsBuff->start == NULL)
        return false;

    if(param.videoCaptureFmt == (unsigned int)V4L2_PIX_FMT_MJPEG)
    {
        return false;
    }
    else if(param.videoCaptureFmt == (unsigned int)V4L2_PIX_FMT_YUV420)
    {
        memcpy(dtsBuff->start,srcBuff->start,srcBuff->length);
        return true;
    }
    else if(param.videoCaptureFmt == (unsigned int)V4L2_PIX_FMT_YUYV)
    {
        ComFunction::yuyv_to_yuv420P((unsigned char*)srcBuff->start,(unsigned char*)dtsBuff->start,param.width,param.height);
    }
    else
        return false;
}

bool CCameraDevice::SaveYUVToJPG(BUFTYPE *yuvBuff, VideoParam param, char *bmpPath)
{
    AVFormatContext* pFormatCtx;
    AVOutputFormat* fmt;
    AVStream* video_st;
    AVCodecContext* pCodecCtx;
    AVCodec* pCodec;

    uint8_t* picture_buf;
    AVFrame* picture;
    int size;
    int in_w=param.width,in_h=param.height;									//宽高
    const char* out_file = bmpPath;					//输出文件路径

    av_register_all();

    pFormatCtx = avformat_alloc_context();
    fmt = av_guess_format("mjpeg", NULL, NULL);
    pFormatCtx->oformat = fmt;

    if (avio_open(&pFormatCtx->pb,out_file, AVIO_FLAG_READ_WRITE) < 0)
    {
        LOG("输出文件打开失败");
        return false;
    }
    video_st = avformat_new_stream(pFormatCtx, 0);
    if (video_st==NULL)
    {
        LOG("avformat_new_stream fail");
        return false;
    }
    pCodecCtx = video_st->codec;
    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUVJ420P;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;

    pCodec = avcodec_find_encoder(pCodecCtx->codec_id);
    if (!pCodec)
    {
        LOG("没有找到合适的编码器！");
        return false;
    }
    if (avcodec_open2(pCodecCtx, pCodec,NULL) < 0)
    {
        LOG("编码器打开失败！");
        return false;
    }
    picture = av_frame_alloc();
    size = avpicture_get_size(pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    picture_buf = (uint8_t *)av_malloc(size);
    if (!picture_buf)
    {
        LOG("av_malloc fail");
        return false;
    }
    avpicture_fill((AVPicture *)picture, picture_buf, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height);
    avformat_write_header(pFormatCtx,NULL);

    AVPacket pkt;
    int y_size = pCodecCtx->width * pCodecCtx->height;
    av_new_packet(&pkt,y_size*3);
    //读入YUV
    int yuvSize = y_size*3/2;
    memcpy(picture_buf,yuvBuff->start,y_size*3/2);

    picture->width = pCodecCtx->width;
    picture->height =  pCodecCtx->height;
    picture->format = pCodecCtx->pix_fmt;

    picture->data[0] = picture_buf;  // 亮度Y
    picture->data[1] = picture_buf+ y_size;  // U
    picture->data[2] = picture_buf+ y_size*5/4; // V

    int got_picture=0;
    int ret = avcodec_encode_video2(pCodecCtx, &pkt,picture, &got_picture);
    if(ret < 0)
    {
        LOG("编码错误！\n");
        return -1;
    }
    if (got_picture==1)
    {
        pkt.stream_index = video_st->index;
        ret = av_write_frame(pFormatCtx, &pkt);
    }

    av_free_packet(&pkt);
    //写文件尾
    av_write_trailer(pFormatCtx);

    LOG("编码成功！\n");

    if (video_st)
    {
        avcodec_close(video_st->codec);
        av_free(picture);
        av_free(picture_buf);
    }
    avio_close(pFormatCtx->pb);
    avformat_free_context(pFormatCtx);

   return true;
}


int CCameraDevice::StreamOff()
{
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if(ioctl(m_camfd,VIDIOC_STREAMOFF,&type) == -1)
    {
        LOG("Fail to ioctl 'VIDIOC_STREAMOFF'");
        return -1;
    }
    return 0;
}

int CCameraDevice::ReleaseMMap()
{
    for(unsigned int i = 0; i < m_buf_num; i++)
    {
        int ret = munmap(m_usr_buf[i].start, m_usr_buf[i].length);
        if (ret < 0)
        {
            LOG("munmap faile");
            return -1;
        }
    }
    free(m_usr_buf);
    m_usr_buf = NULL;
    if(m_yuv_buf->start)
    {
        free(m_yuv_buf->start);
        m_yuv_buf->start = NULL;
    }
    free(m_yuv_buf);
    m_yuv_buf = NULL;
    return 0;
}
void CCameraDevice::run()
{
    if(m_param.frameRate == 0)
    {
        Close();
        return;
    }
    int  ret = InitMMap();
    if(ret < 0)
    {
        Close();
       LOG(QString("mmap_buffer error=%1").arg(ret));
       return;
    }
    ret = StreamOn();
    if(ret < 0)
    {
        Close();
        LOG(QString("StreamOn error=%1").arg(ret));
        return;
    }
    while(!m_stop)
    {
        WriteFrame();
        int usec = 1000*1000/m_param.frameRate;
        usleep(usec);
    }
    ret = StreamOff();
    if(ret < 0)
    {
        LOG(QString("StreamOff error=%1").arg(ret));
    }
    ret = ReleaseMMap();
    if(ret < 0)
    {
        LOG(QString("ReleaseMMap error=%1").arg(ret));
    }
    Close();
}
