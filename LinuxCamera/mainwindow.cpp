#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDesktopServices>
#include <QFileDialog>

bool OnAVDataCallBack(char* buff, int buffsize,VideoParam* videoParam,AudioParam* audioParam,void* userData)
{
    MainWindow* pThis = (MainWindow*)userData;
    if(pThis == NULL)
        return false;

    return pThis->OnAVData(buff,buffsize,videoParam,audioParam);
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();
    m_curCamera = new CCameraDevice();
    m_curAudioCapture = new CAudioDevice();
    m_aacEncode = NULL;
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_timer)
    {
        delete m_timer;
        m_timer = NULL;
    }
    if(m_curCamera)
    {
        delete m_curCamera;
        m_curCamera = NULL;
    }
    if(m_curAudioCapture)
    {
        delete m_curAudioCapture;
        m_curAudioCapture = NULL;
    }
    if(m_aacEncode)
    {
        delete m_aacEncode;
        m_aacEncode = NULL;
    }
}

bool MainWindow::OnAVData(char *buff, int buffsize, VideoParam *videoParam, AudioParam *audioParam)
{
    bool aac = ui->cmbAudioFormat->currentIndex() == 0;
    if(videoParam)
    {
        LOG(QString(" yuv data: buffsize=%1").arg(buffsize));
    }
    if(audioParam)
    {
        if(aac)
        {
            if(m_aacEncode == NULL)
            {
                m_aacEncode = new AACEncodeMgr();
                m_aacEncode->Init(*audioParam);
            }
            char* out = NULL;
            int outSize = 0;
            m_aacEncode->encode(buff,buffsize,out,outSize);
        }
        else
        {

        }
    }
    return true;
}

void MainWindow::ShowResult(QString cxt)
{
    ui->labResult->setText(cxt);
    m_timer->start();
}


void MainWindow::on_btSelectVideo_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("select video file"),"/home/wangning/save/",tr("video files(*.avi *.mp4 *.wmv *.flv);;All files(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->edVideoFile->setText(fileName);
    }
}


void MainWindow::on_btSelectAudio_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("select audio file"),"/home/wangning/save/",tr("audio files(*.aac *.mp3);;All files(*.*)"));
    if(!fileName.isEmpty())
    {
        ui->edAudioFile->setText(fileName);
    }
}


void MainWindow::on_btStartRecord_clicked()
{

}


void MainWindow::on_btStopRecord_clicked()
{

}


void MainWindow::on_btOpenMuxDir_clicked()
{
    QString dirPath = ui->edMuxFile->text();
    QDesktopServices::openUrl(QUrl::fromLocalFile(dirPath+"/"));

}

void MainWindow::initUI()
{
    //init camera
   int index = 0;
   std::list<CameraInfo> lsDev =  m_devMgr.GetCameraList();
   for(std::list<CameraInfo>::iterator it = lsDev.begin(); it != lsDev.end();it++)
   {
       QVariant var=it->devicePath.c_str();
       ui->cmbVideoDev->addItem(it->friendname.c_str(),var);
       index++;
   }
   if(index > 0)
   {
       ui->cmbVideoDev->setCurrentIndex(index);
   }
   //init audio
    std::list<AudioInfo> lsAudio = m_devMgr.GetAudioCaptureList();
    for(std::list<AudioInfo>::iterator it = lsAudio.begin(); it != lsAudio.end();it++)
    {
      QVariant var=it->devPath.c_str();
      ui->cmbAudioDev->addItem(it->friendName.c_str(),var);
    }
    if(lsAudio.size() > 0)
    {
        ui->cmbAudioDev->setCurrentIndex(1);
    }
   m_timer = new QTimer(this);
   m_timer->setInterval(3000);
   connect(m_timer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
}


void MainWindow::on_cmbVideoDev_currentIndexChanged(int index)
{
    ui->edVideoFile->setEnabled(index == 0);
    ui->btSelectVideo->setEnabled(index == 0);
    if(index == 0)
    {
        ui->cmbVideoCaptureFormat->clear();
        ui->cmbVideoPix->clear();
        return;
    }
    ui->cmbVideoCaptureFormat->clear();
    QString curDevName = ui->cmbVideoDev->currentText();
    std::list<CameraFormat>  fmtList = m_devMgr.GetFormatList(curDevName);

    int formatIndex = 0;
    for(std::list<CameraFormat>::iterator it = fmtList.begin(); it != fmtList.end();it++)
    {
        QVariant var=it->format;
        ui->cmbVideoCaptureFormat->addItem(it->foramtFriendName.c_str(),var);
        formatIndex++;
    }
    ui->cmbVideoCaptureFormat->setCurrentIndex(formatIndex-1);
}




void MainWindow::on_cmbVideoCaptureFormat_currentIndexChanged(int)
{
    ui->cmbVideoPix->clear();
    QString curDevName = ui->cmbVideoDev->currentText();
    QString fmtDevname = ui->cmbVideoCaptureFormat->currentText();
    std::list<FrameSize>  frameSizeList = m_devMgr.GetFrameSizeList(curDevName,fmtDevname);
    for(std::list<FrameSize>::iterator it = frameSizeList.begin(); it != frameSizeList.end();it++)
    {
        QString strFrameSize = QString("%1 x %2").arg(it->width).arg(it->height);
        ui->cmbVideoPix->addItem(strFrameSize);
    }
}


void MainWindow::on_btPreview_clicked()
{
    QString strVideoPix = ui->cmbVideoPix->currentText();
    QStringList temp = strVideoPix.split(" x ");
    if(temp.size() < 2)
        return;

    VideoParam param;
    param.renderWidget = ui->previewWidget;
    param.width = temp.at(0).toInt();
    param.height = temp.at(1).toInt();
    param.devPath = ui->cmbVideoDev->currentData().toString().toStdString();
    param.avEncodeFormat = ui->cmbVideoFormat->currentIndex();// 0 h264 1 h265
    param.videoCaptureFmt = ui->cmbVideoCaptureFormat->currentData().toUInt();
    param.encodeDeviceType = ui->cmbVideoEncodeType->currentIndex() ; //0 x264
    param.saveDir = "/home/wangning/save/";
    param.frameRate = 25;
    if(m_curCamera)
    {
        m_curCamera->Open(param,OnAVDataCallBack,this);
    }
    // audio
    AudioParam audioParam;
    AudioInfo audioInfo = m_devMgr.GetAudioInfo(ui->cmbAudioDev->currentText().toStdString());
    if(audioInfo.devPath.empty())
    {
        ShowResult("音频设备打开失败");
        return;
    }
    audioParam.devPath = audioInfo.devPath;
    if(m_curAudioCapture)
    {
        m_curAudioCapture->Open(audioParam,OnAVDataCallBack,this);
    }

}

void MainWindow::OnTimeOut()
{
    ui->labResult->setText("");
    m_timer->stop();
}


void MainWindow::on_btScreenShot_clicked()
{
    if(m_curCamera)
    {
        m_curCamera->ScreensShot();
    }
}


void MainWindow::on_cmbAudioDev_currentIndexChanged(int index)
{
    ui->edAudioFile->setEnabled(index == 0);
    ui->btSelectAudio->setEnabled(index == 0);
}

