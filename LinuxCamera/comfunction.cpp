#include "comfunction.h"
#include "comdef.h"
#include <QDateTime>


const int Table_fv1[256]={ -180, -179, -177, -176, -174, -173, -172, -170, -169, -167, -166, -165, -163, -162, -160, -159, -158, -156, -155, -153, -152, -151, -149, -148, -146, -145, -144, -142, -141, -139, -138, -137, -135, -134, -132, -131, -130, -128, -127, -125, -124, -123, -121, -120, -118, -117, -115, -114, -113, -111, -110, -108, -107, -106, -104, -103, -101, -100, -99, -97, -96, -94, -93, -92, -90, -89, -87, -86, -85, -83, -82, -80, -79, -78, -76, -75, -73, -72, -71, -69, -68, -66, -65, -64, -62, -61, -59, -58, -57, -55, -54, -52, -51, -50, -48, -47, -45, -44, -43, -41, -40, -38, -37, -36, -34, -33, -31, -30, -29, -27, -26, -24, -23, -22, -20, -19, -17, -16, -15, -13, -12, -10, -9, -8, -6, -5, -3, -2, 0, 1, 2, 4, 5, 7, 8, 9, 11, 12, 14, 15, 16, 18, 19, 21, 22, 23, 25, 26, 28, 29, 30, 32, 33, 35, 36, 37, 39, 40, 42, 43, 44, 46, 47, 49, 50, 51, 53, 54, 56, 57, 58, 60, 61, 63, 64, 65, 67, 68, 70, 71, 72, 74, 75, 77, 78, 79, 81, 82, 84, 85, 86, 88, 89, 91, 92, 93, 95, 96, 98, 99, 100, 102, 103, 105, 106, 107, 109, 110, 112, 113, 114, 116, 117, 119, 120, 122, 123, 124, 126, 127, 129, 130, 131, 133, 134, 136, 137, 138, 140, 141, 143, 144, 145, 147, 148, 150, 151, 152, 154, 155, 157, 158, 159, 161, 162, 164, 165, 166, 168, 169, 171, 172, 173, 175, 176, 178 };
const int Table_fv2[256]={ -92, -91, -91, -90, -89, -88, -88, -87, -86, -86, -85, -84, -83, -83, -82, -81, -81, -80, -79, -78, -78, -77, -76, -76, -75, -74, -73, -73, -72, -71, -71, -70, -69, -68, -68, -67, -66, -66, -65, -64, -63, -63, -62, -61, -61, -60, -59, -58, -58, -57, -56, -56, -55, -54, -53, -53, -52, -51, -51, -50, -49, -48, -48, -47, -46, -46, -45, -44, -43, -43, -42, -41, -41, -40, -39, -38, -38, -37, -36, -36, -35, -34, -33, -33, -32, -31, -31, -30, -29, -28, -28, -27, -26, -26, -25, -24, -23, -23, -22, -21, -21, -20, -19, -18, -18, -17, -16, -16, -15, -14, -13, -13, -12, -11, -11, -10, -9, -8, -8, -7, -6, -6, -5, -4, -3, -3, -2, -1, 0, 0, 1, 2, 2, 3, 4, 5, 5, 6, 7, 7, 8, 9, 10, 10, 11, 12, 12, 13, 14, 15, 15, 16, 17, 17, 18, 19, 20, 20, 21, 22, 22, 23, 24, 25, 25, 26, 27, 27, 28, 29, 30, 30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 37, 38, 39, 40, 40, 41, 42, 42, 43, 44, 45, 45, 46, 47, 47, 48, 49, 50, 50, 51, 52, 52, 53, 54, 55, 55, 56, 57, 57, 58, 59, 60, 60, 61, 62, 62, 63, 64, 65, 65, 66, 67, 67, 68, 69, 70, 70, 71, 72, 72, 73, 74, 75, 75, 76, 77, 77, 78, 79, 80, 80, 81, 82, 82, 83, 84, 85, 85, 86, 87, 87, 88, 89, 90, 90 };
const int Table_fu1[256]={ -44, -44, -44, -43, -43, -43, -42, -42, -42, -41, -41, -41, -40, -40, -40, -39, -39, -39, -38, -38, -38, -37, -37, -37, -36, -36, -36, -35, -35, -35, -34, -34, -33, -33, -33, -32, -32, -32, -31, -31, -31, -30, -30, -30, -29, -29, -29, -28, -28, -28, -27, -27, -27, -26, -26, -26, -25, -25, -25, -24, -24, -24, -23, -23, -22, -22, -22, -21, -21, -21, -20, -20, -20, -19, -19, -19, -18, -18, -18, -17, -17, -17, -16, -16, -16, -15, -15, -15, -14, -14, -14, -13, -13, -13, -12, -12, -11, -11, -11, -10, -10, -10, -9, -9, -9, -8, -8, -8, -7, -7, -7, -6, -6, -6, -5, -5, -5, -4, -4, -4, -3, -3, -3, -2, -2, -2, -1, -1, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 20, 21, 21, 22, 22, 22, 23, 23, 23, 24, 24, 24, 25, 25, 25, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 39, 39, 39, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43 };
const int Table_fu2[256]={ -227, -226, -224, -222, -220, -219, -217, -215, -213, -212, -210, -208, -206, -204, -203, -201, -199, -197, -196, -194, -192, -190, -188, -187, -185, -183, -181, -180, -178, -176, -174, -173, -171, -169, -167, -165, -164, -162, -160, -158, -157, -155, -153, -151, -149, -148, -146, -144, -142, -141, -139, -137, -135, -134, -132, -130, -128, -126, -125, -123, -121, -119, -118, -116, -114, -112, -110, -109, -107, -105, -103, -102, -100, -98, -96, -94, -93, -91, -89, -87, -86, -84, -82, -80, -79, -77, -75, -73, -71, -70, -68, -66, -64, -63, -61, -59, -57, -55, -54, -52, -50, -48, -47, -45, -43, -41, -40, -38, -36, -34, -32, -31, -29, -27, -25, -24, -22, -20, -18, -16, -15, -13, -11, -9, -8, -6, -4, -2, 0, 1, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 21, 23, 24, 26, 28, 30, 31, 33, 35, 37, 39, 40, 42, 44, 46, 47, 49, 51, 53, 54, 56, 58, 60, 62, 63, 65, 67, 69, 70, 72, 74, 76, 78, 79, 81, 83, 85, 86, 88, 90, 92, 93, 95, 97, 99, 101, 102, 104, 106, 108, 109, 111, 113, 115, 117, 118, 120, 122, 124, 125, 127, 129, 131, 133, 134, 136, 138, 140, 141, 143, 145, 147, 148, 150, 152, 154, 156, 157, 159, 161, 163, 164, 166, 168, 170, 172, 173, 175, 177, 179, 180, 182, 184, 186, 187, 189, 191, 193, 195, 196, 198, 200, 202, 203, 205, 207, 209, 211, 212, 214, 216, 218, 219, 221, 223, 225 };



ComFunction::ComFunction(QObject *parent) : QObject(parent)
{

}

void ComFunction::WriteLog(QString strMsg, QString fileName, QString line)
{
    QString strTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString strLog = QString("%1 %2:(%3)  %4").arg(strTime)
            .arg(fileName).arg(line).arg(strMsg);
    QString debug = QString("%1 %2").arg(strTime).arg(strMsg);
    qInfo(strLog.toStdString().c_str());
}

void ComFunction::MJPEGToRGB(unsigned char *data, unsigned int dataSize, unsigned char *outBuffer)
{
    AVPacket *avPkt = av_packet_alloc();
    avPkt->size = dataSize;
    avPkt->data = data;

    AVCodec *mjpegCodec = avcodec_find_decoder(AV_CODEC_ID_MJPEG);
    AVCodecContext* codecCtx = avcodec_alloc_context3(mjpegCodec);
    avcodec_get_context_defaults3(codecCtx, mjpegCodec);
    avcodec_open2(codecCtx, mjpegCodec, nullptr);

    auto ret = avcodec_send_packet(codecCtx, avPkt);
    if (ret >=0) {
        AVFrame* YUVFrame = av_frame_alloc();
        ret = avcodec_receive_frame(codecCtx, YUVFrame);
        if (ret >= 0)
        {

            AVFrame* RGB24Frame = av_frame_alloc();
            struct SwsContext* convertCxt = sws_getContext(
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_YUV420P,
                YUVFrame->width, YUVFrame->height, AV_PIX_FMT_RGB24,
                SWS_POINT, NULL, NULL, NULL
            );
            av_image_fill_arrays(
                RGB24Frame->data, RGB24Frame->linesize, outBuffer,
                AV_PIX_FMT_RGB24, YUVFrame->width, YUVFrame->height,
                1
            );
            sws_scale(convertCxt, YUVFrame->data, YUVFrame->linesize, 0, YUVFrame->height, RGB24Frame->data, RGB24Frame->linesize);

            sws_freeContext(convertCxt);
            av_frame_free(&RGB24Frame);
        }
        av_frame_free(&YUVFrame);
    }
    av_packet_unref(avPkt);
    av_packet_free(&avPkt);
    avcodec_free_context(&codecCtx);
}

int ComFunction::GetYUVLen(int width, int height, int format)
{
    if(width <= 0 || height <=0)
        return 0;
    int yuvLen = 0;
    if(format == SDL_PIXELFORMAT_IYUV)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_RGB24)
    {
        yuvLen = width*height*3;
    }
    else if(format == SDL_PIXELFORMAT_BGR24)
    {
        yuvLen = width*height*3;
    }
    else if(format == SDL_PIXELFORMAT_RGBA32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_ARGB32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_BGRA32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_ABGR32)
    {
        yuvLen = width*height*4;
    }
    else if(format == SDL_PIXELFORMAT_YV12)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_YUY2)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_UYVY)
    {
        yuvLen = width*height*2;
    }
    else if(format == SDL_PIXELFORMAT_YVYU)
    {
        yuvLen = width*height*2;
    }
    else if(format == SDL_PIXELFORMAT_NV12)
    {
        yuvLen = width*height + width*height/2;
    }
    else if(format == SDL_PIXELFORMAT_NV21)
    {
        yuvLen = width*height + width*height/2;
    }
    return yuvLen;
}

int ComFunction::GetRowPixel(int width, int height, int format)
{
    int rowPixel = 0;
    if(format == SDL_PIXELFORMAT_IYUV)
    {
        return width;
    }
    else if(format == SDL_PIXELFORMAT_RGB24 || format == SDL_PIXELFORMAT_BGR24)
    {
        rowPixel = width*3;
    }
    else if(format == SDL_PIXELFORMAT_RGBA32 || format == SDL_PIXELFORMAT_ARGB32 || format == SDL_PIXELFORMAT_BGRA32 || format == SDL_PIXELFORMAT_ABGR32)
    {
        rowPixel = width*4;
    }
    else if(format == SDL_PIXELFORMAT_YV12)
    {
        return width;
    }
    else if(format == SDL_PIXELFORMAT_YUY2)
    {
        rowPixel = width + width/2;
    }
    else if(format == SDL_PIXELFORMAT_UYVY)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_YVYU)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_NV12)
    {
        rowPixel = width*2;
    }
    else if(format == SDL_PIXELFORMAT_NV21)
    {
        rowPixel = width*2;
    }
    return rowPixel;
}
void ComFunction::yuyv_to_yuv420P(uint8_t *in, uint8_t*out, int width, int height)
{
    uint8_t *y, *u, *v;
    int i, j, offset = 0, yoffset = 0;

    y = out;                                    //yuv420的y放在前面
    u = out + (width * height);                 //yuv420的u放在y后
    v = out + (width * height * 5 / 4);         //yuv420的v放在u后

    for(j = 0; j < height; j++)
    {
        yoffset = 2 * width * j;
        for(i = 0; i < width * 2; i = i + 4)
        {
            offset = yoffset + i;
            *(y ++) = *(in + offset);
            *(y ++) = *(in + offset + 2);
            if(j % 2 == 1)                      //抛弃奇数行的UV分量
            {
                *(u ++) = *(in + offset + 1);
                *(v ++) = *(in + offset + 3);
            }
        }
    }
}

