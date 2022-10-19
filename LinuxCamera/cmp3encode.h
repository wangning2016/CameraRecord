#ifndef CMP3ENCODE_H
#define CMP3ENCODE_H

#include <QObject>
#include "device_def.h"

class CMP3Encode : public QObject , public AudioEncode
{
    Q_OBJECT
public:
    explicit CMP3Encode(QObject *parent = nullptr);

    virtual void Init(AudioParam param);
    virtual bool encode(char* buff, int buffsize,char* out, int& outSize);
signals:

};

#endif // CMP3ENCODE_H
