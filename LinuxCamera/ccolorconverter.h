#ifndef CCOLORCONVERTER_H
#define CCOLORCONVERTER_H

#include <QObject>

class CColorConverter : public QObject
{
    Q_OBJECT
public:
    explicit CColorConverter(QObject *parent = nullptr);


    static void yuyv_to_rgb888(unsigned char* yuv,unsigned char* rgb, int imgWidth, int imgHeight);
signals:

};

#endif // CCOLORCONVERTER_H
