#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cdevicemgr.h"
#include "ccameradevice.h"
#include "caudiodevice.h"
#include "aacencodemgr.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool OnAVData(char* buff, int buffsize,VideoParam* videoParam,AudioParam* audioParam);

    void ShowResult(QString cxt);
private slots:
    void on_btSelectVideo_clicked();

    void on_btSelectAudio_clicked();

    void on_btStartRecord_clicked();

    void on_btStopRecord_clicked();

    void on_btOpenMuxDir_clicked();

    void on_cmbVideoDev_currentIndexChanged(int index);

    void on_cmbVideoCaptureFormat_currentIndexChanged(int index);

    void on_btPreview_clicked();

    void OnTimeOut();
    void on_btScreenShot_clicked();

    void on_cmbAudioDev_currentIndexChanged(int index);

private:
    void initUI();
    Ui::MainWindow *ui;
    CDeviceMgr m_devMgr;

    CCameraDevice* m_curCamera;
    CAudioDevice* m_curAudioCapture;


    AACEncodeMgr* m_aacEncode;


private:
    QTimer* m_timer;
};
#endif // MAINWINDOW_H
