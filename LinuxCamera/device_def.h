#ifndef DEVICE_DEF_H
#define DEVICE_DEF_H
#include "comdef.h"

#define avEncodeFormat_h264 0
#define avEncodeFormat_h265 1





// 分辨率
struct FrameSize
{
    unsigned int width;
    unsigned int height;

    FrameSize()
    {
        width = 0;
        height = 0;
    }
};
// 摄像头所支持的分辨率
struct CameraFormat
{
    unsigned int format;
    std::string foramtFriendName;
    std::list<FrameSize> frameSizeList;

    CameraFormat()
    {
        format = 0;
    }
};

struct CameraInfo
{
    std::list<CameraFormat> camareFormatList;
    string driver;
    string friendname;
    string devicePath;
    string businfo;
};

struct AudioParam
{
    std::string devPath;
    int rate;//采样频率
    int channel;//声道数
    int pcmFormat;//pcm格式
    int nb_samples;//采样数
    AudioParam()
    {
        rate = 44100;
        channel = 2;
        pcmFormat = SND_PCM_FORMAT_S16_LE;
        nb_samples = 1024;
    }
};

struct VideoParam
{
    std::string saveDir;
    std::string devPath;
    unsigned int videoCaptureFmt;
    int avEncodeFormat;
    int encodeDeviceType;
    int width;
    int height;
    QWidget* renderWidget;
    int frameRate;

    VideoParam()
    {
        frameRate = 0;
        videoCaptureFmt = 0;
        avEncodeFormat = 0;
        encodeDeviceType = 0;
        width = 0;
        height = 0;
        renderWidget = NULL;
    }
};

struct AudioInfo
{
    std::string devPath;
    std::string friendName;
    std::string cardName;
    std::string devicename;
};

typedef struct BUFTYPE
{
    void *start;
    int length;

    BUFTYPE()
    {
        start = NULL;
        length = 0;
    }
}*usr_buf;




typedef bool (*pf_AVDataCallBack)(char* buff, int buffsize,VideoParam* videoParam,AudioParam* audioParam,void* userData);

#endif // DEVICE_DEF_H
