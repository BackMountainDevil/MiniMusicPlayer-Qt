#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("极简音乐播放器");
    voi = new QMediaPlayer(this);
    //voi->setMedia(QUrl::fromLocalFile("../10-Multimedia/res/voi/legend.mp3"));
    qDebug("voi ready");
    ui->pushButton_4->setEnabled(false);
}

MainWindow::~MainWindow()
{    
    delete ui;
}

//一个按键实现播放暂停两个操作
void MainWindow::on_pushButton_4_clicked()
{
    //如果没有在播放
    if(this->isplay)
    {
        voi->stop();
        qDebug("voi stop");
        this->ui->pushButton_4->setText(tr("播放"));
        this->isplay = ! this->isplay;
     }
    else    //如果音频正在播放
    {
        voi->play();
        qDebug("voi play");
        this->ui->pushButton_4->setText(tr("暂停"));
        this->isplay = ! this->isplay;
    }

}

//选取要播放的音乐文件
void MainWindow::on_pushButton_clicked()
{
    //筛选文件，只能选择mp3或wav格式的文件
    QUrl path =  QFileDialog::getOpenFileUrl(this,tr("请选择要播放的音乐"),QUrl( "C:/"),"music(*.mp3 *.wav )");
    //qDebug()<<path;

    //选取文件后自动播放
    if(!path.isEmpty())
    {   qDebug("voi file ready");
        voi->setMedia(path);
        ui->pushButton_4->setEnabled(true);


        voi->play();
        qDebug("voi play");
        this->ui->pushButton_4->setText(tr("暂停"));
        this->isplay = ! this->isplay;
    }
    else    //没有选取音乐就提示
    {
       qDebug("voi file not choose");
       QMessageBox::warning(this,tr("温馨提示"),tr("你没有选择音乐文件\n请重新选择"));
    }

}
