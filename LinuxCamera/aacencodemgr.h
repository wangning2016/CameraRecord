#ifndef AACENCODEMGR_H
#define AACENCODEMGR_H

#include <QObject>
#include "device_def.h"
#include "aacencoder.h"
#include "audioresampler.h"

class AACEncodeMgr : public QObject , public AudioEncode
{
    Q_OBJECT
public:
    explicit AACEncodeMgr(QObject *parent = nullptr);

    virtual void Init(AudioParam param);
    virtual bool encode(char* buff, int buffsize,char* out, int& outSize);
signals:


private:
    CAACEncoder m_encode;
    AudioParam m_param;
    BUFTYPE m_pcmBuff;
    AudioResampler m_audioResampler;
};

#endif // AACENCODEMGR_H
