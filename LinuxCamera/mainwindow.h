#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btSelectVideo_clicked();

    void on_btSelectAudio_clicked();

    void on_btStartRecord_clicked();

    void on_btStopRecord_clicked();

    void on_btOpenMuxDir_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
