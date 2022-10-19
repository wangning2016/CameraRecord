#include "cdevicemgr.h"
#include "comdef.h"

CDeviceMgr::CDeviceMgr(QObject *parent) : QObject(parent)
{
    InitCamera();
    InitAudio();
}

std::list<CameraInfo> CDeviceMgr::GetCameraList()
{
    return m_cameraList;
}

std::list<CameraFormat> CDeviceMgr::GetFormatList(QString devFriendName)
{
    for(std::list<CameraInfo>::iterator it = m_cameraList.begin(); it != m_cameraList.end();it++)
    {
        QString devName = it->friendname.c_str();
        if(devName == devFriendName)
        {
            return it->camareFormatList;
        }
    }
    std::list<CameraFormat> tmp;
    return tmp;
}

std::list<FrameSize> CDeviceMgr::GetFrameSizeList(QString devFriendName, QString foramtFriendName)
{
    std::list<CameraFormat> fmtList = GetFormatList(devFriendName);
    for(std::list<CameraFormat>::iterator it = fmtList.begin(); it != fmtList.end();it++)
    {
        QString fmtName = it->foramtFriendName.c_str();
        if(fmtName == foramtFriendName)
        {
            return it->frameSizeList;
        }
    }
    std::list<FrameSize> tmp;
    return tmp;
}

std::list<AudioInfo> CDeviceMgr::GetAudioCaptureList()
{
    return m_audioCaptureList;
}

AudioInfo CDeviceMgr::GetAudioInfo(string friendName)
{
    for(std::list<AudioInfo>::iterator it = m_audioCaptureList.begin(); it != m_audioCaptureList.end();it++)
    {
        if(it->friendName == friendName)
        {
            return *it;
        }
    }
    AudioInfo  tmp;
    return tmp;
}

void CDeviceMgr::InitCamera()
{
    QString cmd = "ls /dev/video*";
    FILE* pDev = popen(cmd.toStdString().c_str(), "r");
    if(pDev == NULL)
        return;
    char buffer[500] = {0};
    int index = 0;
    while(fgets(buffer,500,pDev) != nullptr)
    {
        QString videoPath = QString::fromLocal8Bit(buffer);
        CameraInfo videoInfo;
        videoPath = videoPath.replace("\n","");
        videoPath = videoPath.replace("\r","");
        videoInfo.devicePath = videoPath.toStdString();
        int cam_fd = open(videoPath.toStdString().c_str(), O_RDWR);
        if(cam_fd < 0)
        {
            continue;
        }
        if(0 != ioctl(cam_fd, VIDIOC_S_INPUT, &index))
        {
            close(cam_fd);
            continue;
        }
        struct v4l2_capability cap;
        ioctl(cam_fd, VIDIOC_QUERYCAP, &cap);
        videoInfo.driver = (char*)cap.driver;
        videoInfo.businfo = (char*)cap.bus_info;
        videoInfo.friendname = (char*)cap.card;


        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        struct v4l2_fmtdesc fmtdesc;
        struct v4l2_frmsizeenum frmsize;
        fmtdesc.index = 0;
        fmtdesc.type = type;
        while (ioctl(cam_fd, VIDIOC_ENUM_FMT, &fmtdesc) >= 0)
        {
            CameraFormat cameraFmt;
            cameraFmt.foramtFriendName = (char*)fmtdesc.description;
            cameraFmt.format = fmtdesc.pixelformat;
            frmsize.pixel_format = fmtdesc.pixelformat;
            frmsize.index = 0;
            while (ioctl(cam_fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0)
            {
                FrameSize framsize;
                if(frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE)
                {
                    framsize.height = frmsize.discrete.height;
                    framsize.width = frmsize.discrete.width;
                }
                frmsize.index++;
                cameraFmt.frameSizeList.push_back(framsize);
            }
            videoInfo.camareFormatList.push_back(cameraFmt);
            fmtdesc.index++;
        }
        m_cameraList.push_back(videoInfo);
        close(cam_fd);
    }
    pclose(pDev);
}

void CDeviceMgr::InitAudio()
{
   m_audioCaptureList =  GetAudioDev(SND_PCM_STREAM_CAPTURE);
}

std::list<AudioInfo> CDeviceMgr::GetAudioDev(snd_pcm_stream_t stream)
{
    std::list<AudioInfo>  audioList;

    snd_ctl_t *handle;
    int card, err, dev, idx;
    snd_ctl_card_info_t *info;
    snd_pcm_info_t *pcminfo;
    snd_ctl_card_info_alloca(&info);
    snd_pcm_info_alloca(&pcminfo);
    card = -1;
    if (snd_card_next(&card) < 0 || card < 0)
    {
        LOG("no soundcards found...");
        return audioList;
    }
    snd_pcm_stream_name(stream);
    while (card >= 0)
    {
        char name[32];
        sprintf(name, "hw:%d", card);

        if ((err = snd_ctl_open(&handle, name, 0)) < 0)
        {
            LOG(QString("control open (%1): %2").arg(card).arg(snd_strerror(err)));
            goto next_card;
        }
        if ((err = snd_ctl_card_info(handle, info)) < 0)
        {
             LOG(QString("control hardware info (%1): %2").arg(card).arg(snd_strerror(err)));
             snd_ctl_close(handle);
            goto next_card;
        }
        dev = -1;
        while (1)
        {
            unsigned int count;
            if (snd_ctl_pcm_next_device(handle, &dev)<0)
                LOG("snd_ctl_pcm_next_device");
            if (dev < 0)
                break;
            snd_pcm_info_set_device(pcminfo, dev);
            snd_pcm_info_set_subdevice(pcminfo, 0);
            snd_pcm_info_set_stream(pcminfo, stream);

            if ((err = snd_ctl_pcm_info(handle, pcminfo)) < 0)
            {
                if (err != -ENOENT)
                    LOG(QString("control digital audio info (%1): %2").arg(card).arg(snd_strerror(err)));
                continue;

            }

            AudioInfo audioInfo;
            char devpath[100];
            sprintf(devpath,"hw:%d,%d",card,dev);
            audioInfo.devPath = devpath;
            audioInfo.cardName = snd_ctl_card_info_get_name(info);
            audioInfo.devicename = snd_pcm_info_get_name(pcminfo);
            std::string friendName = audioInfo.cardName + ":[";
            friendName += audioInfo.devicename + "](";
            friendName += audioInfo.devPath + ")";
            audioInfo.friendName = friendName;
            audioList.push_back(audioInfo);

        }
        snd_ctl_close(handle);
next_card:
        if (snd_card_next(&card) < 0)
        {
            break;
        }
    }
    return audioList;

}
