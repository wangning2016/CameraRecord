#ifndef CCAMERADEVICE_H
#define CCAMERADEVICE_H

#include <QThread>
#include <QWidget>
#include "device_def.h"
#include "comfunction.h"
#include "comdef.h"
#include <SDL2/SDL.h>

class CCameraDevice : public QThread
{
    Q_OBJECT
public:
    explicit CCameraDevice(QObject *parent = nullptr);
    ~CCameraDevice();

    bool Open(VideoParam param,pf_AVDataCallBack callBack,void* userData);
    void Close();
    void ScreensShot();

    virtual void run();
signals:

private:
    int InitMMap();
    int ReleaseMMap();
    int StreamOff();
    int StreamOn();
    int WriteFrame();


    bool Convert2YUV420(BUFTYPE* srcBuff, VideoParam param, BUFTYPE* dtsBuff);
    bool SaveYUVToJPG(BUFTYPE* yuvBuff,VideoParam param, char* bmpPath);
private:
    pf_AVDataCallBack m_avCallBack;
    void* m_userData;
    VideoParam m_param;
    int m_camfd;
    bool m_stop;
    bool m_screenshot;


    unsigned int m_buf_num;//指定缓冲区个数
    BUFTYPE* m_usr_buf;
    BUFTYPE* m_yuv_buf;
    BUFTYPE m_bmpBuff;

    SDL_Window* m_window;
    SDL_Renderer* m_render;
    SDL_Texture* m_texture;
    SDL_Thread* m_thread;
};

#endif // CCAMERADEVICE_H
