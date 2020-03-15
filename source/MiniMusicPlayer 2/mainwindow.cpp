#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QMediaPlaylist>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>
#include <QMimeDatabase>
#include <QCoreApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initplayer();
}

MainWindow::~MainWindow()
{    
    delete ui;
}

//界面ui未完成的在这里设计ui和逻辑初始化设置
void MainWindow::initplayer()
{
    ui->pushButton_4->setEnabled(false);    //未选择文件不让播放
    this->setAcceptDrops(true);     //打开接受拖动事件
//    this->setMouseTracking(true);
    voi = new QMediaPlayer(this);
    voi->setVolume(volume);
    voilist = new QMediaPlaylist;
    voilist->setPlaybackMode(QMediaPlaylist::Loop); //默认循环播放列表中的歌曲
    qDebug("voi ready to receive file");
    this->list = new  ListWidget (this);    //创建播放菜单
    qDebug("creating ListWidget");
    this->ui->toolButton->hide();   //暂时取消重置歌单的功能
}

//实现音乐的播放和按钮转换
void MainWindow::voiplay()
{
    voi->play();
    qDebug("voi play");
    //this->ui->pushButton_4->setText(tr("暂停"));
     this->ui->pushButton_4->setToolTip("暂停");
    this->ui->pushButton_4->setIcon(QIcon(":/img/play.png"));   //图标的更换
}

//实现暂停的播放和按钮转换
void MainWindow::voistop()
{
    voi->pause();   //暂停，会从暂停的位置开始播放
    //voi->stop();  //结束，会从头开始播放   
    qDebug("voi stop");
    //this->ui->pushButton_4->setText(tr("播放"));
    this->ui->pushButton_4->setToolTip("播放");
    this->ui->pushButton_4->setIcon(QIcon(":/img/pause.png"));

}

//播放暂停按键状态识别
void MainWindow::on_pushButton_4_clicked()
{
    //如果没有在播放
    if(voi->state() == QMediaPlayer::PlayingState)
    {
        this->voistop();
    }
    else
    {
        this->voiplay();
    }

}

//对话框选取要播放的音乐文件并将文件名添加到歌单窗口
//有的文件名识别标题不是很正确，比如“山海-草东没有排队.mp3”识别出来是"10_山海"
void MainWindow::on_pushButton_clicked()
{
    //选择的时候筛选文件，只能选择mp3、wav、mpga格式的文件
    QList<QUrl> path =  QFileDialog::getOpenFileUrls(this,tr("请选择要播放的音乐"),QUrl( "C:/"),"music(*.mp3 *.wav *.mpga)");
    qDebug()<<"path"<<path;

    QMediaPlayer tmpPlayer;//解析文件信息的临时播放器
    foreach(QUrl i,path )
    {
        voilist->addMedia(i);   //添加到播放列表
        qDebug("voi file url adding");

        tmpPlayer.setMedia(i);  //将歌名添加到歌单窗口
        while(!tmpPlayer.isMetaDataAvailable())
        {
            QCoreApplication::processEvents();
        }
        //其它信息暂时不添加
        if(tmpPlayer.isMetaDataAvailable())
        {
//            author = tempPlayer.metaData(QStringLiteral("Author")).toStringList().join(",");
//            albumTitle = tempPlayer.metaData(QStringLiteral("AlbumTitle")).toString();
//            audioBitRate = tempPlayer.metaData(QStringLiteral("AudioBitRate")).toInt();
            list->additem(tmpPlayer.metaData(QStringLiteral("Title")).toString() );    //将歌曲添加到默认播放列表
        }

    }
    //选取文件后自动播放
    if(!path.isEmpty())
    {   qDebug("voi file ready");

        //正在播放或暂停状态添加新文件取消从头播放，改为继续当前的播放
        if(voi->state() != QMediaPlayer::PlayingState && voi->state() != QMediaPlayer::PausedState)
        {
            //qDebug()<<voi->state();
            voi->setPlaylist(voilist);  //播放过程重定向voilist会导致从头开始播放
            this->voiplay();
            ui->pushButton_4->setEnabled(true);//恢复按钮的功能
        }

    }
    else    //没有选取音乐就提示
    {
       qDebug("voi file not choose");
       QMessageBox::warning(this,tr("温馨提示"),tr("你没有选择音乐文件\n请重新选择"));
    }

}

//实现拖动多个文件的响应
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        qDebug("准备接收拖动文件");
        list->hide();   //拖动添加新文件时隐藏歌单窗口
    }
    else
    {
        event->ignore();
        qDebug("忽略拖动文件");
        list->show();   //未能如愿的显示刚隐藏的窗口！！！！！！
    }
}

//从拖动的文件中提取urls进行播放
void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimedata = event->mimeData();

    if(mimedata->hasUrls())
    {
       //qDebug()<<"接收的文件中的urls："<<mimedata->urls();
       QList <QUrl> path = mimedata->urls();        //获取全部文件的url
       //qDebug()<<"即将播放的url"<<path[0];

       //循环添加到播放列表
       //比for循环好用的foreach，第一次用，研究了不少时间咋用
       QMediaPlayer tmpPlayer;//解析文件信息的临时播放器
       QMimeDatabase db;
       foreach(QUrl i,path )
       {
           //后台筛选音频文件的格式，也可以用辨别后缀名的方式，但是既然有更先进的方法为什么还要自己造轮子呢？

           QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
           if(mime.name()=="audio/mpeg" || mime.name()=="audio/wav")//格式是mp3\mpeg\wav就添加到播放列表
           {
               voilist->addMedia(i);    //添加到播放列表
               qDebug("voi file url adding");

               tmpPlayer.setMedia(i);  //将歌名添加到歌单窗口
               while(!tmpPlayer.isMetaDataAvailable())
               {
                   QCoreApplication::processEvents();
               }
               if(tmpPlayer.isMetaDataAvailable())
               {
                   list->additem(tmpPlayer.metaData(QStringLiteral("Title")).toString() );    //将歌曲添加到默认播放列表
               }
           }
           else//其它格式就不添加
           {
               qDebug("----voi file ERROR！！！----");
           }
       }
       //将新加入的第一个文件作为即将播放的音频
       qDebug("voi file url added");
       if(voi->state() != QMediaPlayer::PlayingState && voi->state() != QMediaPlayer::PausedState)
       {
           voi->setPlaylist(voilist);
           qDebug("voilist loaded");

           this->voiplay();
           ui->pushButton_4->setEnabled(true);//   恢复按钮的功能
       }

    }
    else    //没有选取文件
    {
       qDebug("voi file not choose");
       QMessageBox::warning(this,tr("温馨提示"),tr("你没有选择音乐文件\n请重新选择"));
    }
}

//点击按钮后显示或者关闭播放列表
void MainWindow::on_pushButton_2_clicked()
{
    //设置播放列表默认展示在播放器下方,窗体高度默认300，宽度和播放器一致 this->frameGeometry().height() +
    list->setGeometry(this->geometry().x(),this->geometry().y() + this->height(),this->width(),300);
    if(list->isHidden())
    {
        list->show();
        this->ui->pushButton_2->setToolTip("隐藏歌单");
        this->activateWindow(); //将焦点回归到主界面，不然没有”查看歌单“的提示
    }
    else
    {
        list->hide();
        this->ui->pushButton_2->setToolTip("查看歌单");
    }
}

//重置播放列表，待完善！！！！！！！！！！！！！！！！！！！！！
void MainWindow::on_toolButton_clicked()
{
     qDebug("Resetting the voilist");
     this->voistop();
     //this->voilist->
     this->ui->pushButton_4->setEnabled(false);    //不让播放
     qDebug()<<voi->state();
}
/*
void MainWindow::mouseMoveEvent(QMouseEvent *event) //鼠标拖动窗体时隐藏歌单，拖动完成后在显示
{
    qDebug("dragg");
    if(event->buttons() & Qt::LeftButton)
    {
        qDebug("dragging");
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    qDebug()<<event->button();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

}
*/

//鼠标滚轮上下滚动实现音量调节
void MainWindow::wheelEvent(QWheelEvent *event)
{
    //qDebug()<<event->delta();
    if(event->delta() > 0 && volume < 100)    //实测滚轮向上调大
    {
        volume +=  5;
         voi->setVolume(volume);
        qDebug("volum up");
    }
    else if(event->delta() < 0 && volume >0 )   //实测向下调很容易
    {
        volume -= 5;
        voi->setVolume(volume);
        qDebug("volum down");
    }
}
