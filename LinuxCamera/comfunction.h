#ifndef COMFUNCTION_H
#define COMFUNCTION_H

#include <QObject>

#define LOG(x) SYSLOG(x,__FILE__,__LINE__)
#define SYSLOG(x,file,line) ComFunction::WriteLog(x,QString(file),QString("%1").arg(line))


class ComFunction : public QObject
{
    Q_OBJECT
public:
    explicit ComFunction(QObject *parent = nullptr);

    static void MJPEGToRGB(unsigned char *data, unsigned int dataSize, unsigned char *outBuffer);
    static int GetYUVLen(int width,int height,int format);
    static int GetRowPixel(int width,int height,int format);

    static void yuyv_to_yuv420P(uint8_t *in, uint8_t*out, int width, int height);

    static void WriteLog(QString strMsg,QString fileName,QString line);
signals:

};

#endif // COMFUNCTION_H
