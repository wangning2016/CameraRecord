#ifndef CDEVICEMGR_H
#define CDEVICEMGR_H

#include <QObject>
#include "device_def.h"

class CDeviceMgr : public QObject
{
    Q_OBJECT
public:
    explicit CDeviceMgr(QObject *parent = nullptr);
    //video
    std::list<CameraInfo> GetCameraList();

    std::list<CameraFormat> GetFormatList(QString devFriendName);
    std::list<FrameSize> GetFrameSizeList(QString devFriendName,QString foramtFriendName);

    //audio
    std::list<AudioInfo> GetAudioCaptureList();
    AudioInfo GetAudioInfo(std::string friendName);

private:
    std::list<AudioInfo> GetAudioDev(snd_pcm_stream_t stream);
private:
    void InitCamera();
    void InitAudio();
signals:


private:
    std::list<CameraInfo> m_cameraList;
    std::list<AudioInfo> m_audioCaptureList;
};

#endif // CDEVICEMGR_H
