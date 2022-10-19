#ifndef CAUDIODEVICE_H
#define CAUDIODEVICE_H

#include <QThread>
#include"device_def.h"

class CAudioDevice : public QThread
{
    Q_OBJECT
public:
    explicit CAudioDevice(QObject *parent = nullptr);
    ~CAudioDevice();


    bool Open(AudioParam param,pf_AVDataCallBack callBack,void* userData);
    void Stop();

    virtual void run();

private:
    int GetSampleByte(AudioParam param);
    void Close();
signals:


private:
    pf_AVDataCallBack m_avCallBack;
    AudioParam m_param;
    bool m_stop;
    void* m_userData;

private:
    snd_pcm_t *pcm_st;
    snd_pcm_uframes_t frames;   // 每帧采样点数

    BUFTYPE m_pcm_buf;
};

#endif // CAUDIODEVICE_H
