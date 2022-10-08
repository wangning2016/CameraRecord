
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btSelectVideo_clicked()
{



}


void MainWindow::on_btSelectAudio_clicked()
{

}


void MainWindow::on_btStartRecord_clicked()
{

}


void MainWindow::on_btStopRecord_clicked()
{

}


void MainWindow::on_btOpenMuxDir_clicked()
{

}

