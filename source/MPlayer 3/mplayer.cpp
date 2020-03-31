#include "mplayer.h"
#include "ui_mplayer.h"
#include "QStyleSheet.h"
#include <QDebug>
#include <QFileDialog>
#include <QUrl>
#include <QStyle>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QMimeDatabase>
#include <QMimeType>

/*
 * qDebug()大部分是为了调试程序，单元测试成功后建议注释保留便于后期改进
 *
 * */
MPlayer::MPlayer(QWidget *parent)
    : QWidget(parent,Qt::CustomizeWindowHint)//去掉标题栏和边框，拖动和大小改变无效
    , ui(new Ui::MPlayer)
{
    ui->setupUi(this);    
    setStyleSheet(initQStyle());  //设置Qs样式
    setAcceptDrops(true); //开启拖动功能
    //setAttribute(Qt::WA_TranslucentBackground);//设置窗口背景透明
    ui->btnpp->setIcon(QIcon(":/image/image/play.png"));
    ui->btnopen->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon ));
    ui->btnnext->setIcon(QIcon(":/image/image/next.png"));
    ui->btnpre->setIcon(QIcon(":/image/image/pre.png"));
    ui->btnmax->setIcon(QIcon(":/image/image/max.png"));
    ui->btnmin->setIcon(QIcon(":/image/image/min.png"));
    ui->btnquit->setIcon(QIcon(":/image/image/close.png"));
    ui->labico->setPixmap(QPixmap(":/image/image/ico.png"));
    ui->btnmute->setIcon(QIcon(":/image/image/volume.png"));
    ui->btnpmode->setIcon(QIcon(":/image/image/loop.png"));
    ui->btnopen->setIcon(QIcon(":/image/image/openfile.png"));
    ui->labsimag->setPixmap(QPixmap(":/image/image/designer.jpg"));


    mplayer = new QMediaPlayer(this);
    mplayer->setVolume(50);
    ui->slivol->setValue(this->mplayer->volume());

    mplist = new QMediaPlaylist(this);
    mplist->setPlaybackMode(QMediaPlaylist::Loop); //默认循环播放列表中的歌曲
    mplayer->setPlaylist(mplist);

    //使进度条随着播放自动滑动和修改当前播放位置
    connect(mplayer, &QMediaPlayer::positionChanged, this, &MPlayer::updatePosition);
    //更新进度条和范围和歌曲播放时长
    connect(mplayer, &QMediaPlayer::durationChanged, this, &MPlayer::updateDuration);
    connect(mplayer, &QMediaObject::metaDataAvailableChanged, this, &MPlayer::updateInfo);
    connect(mplayer, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),this, &MPlayer::handleError);

//     this->ui->btnpp->setToolTip(tr("Open file"));
    //打开文件，获取url
    connect(ui->btnopen,&QAbstractButton::clicked,this, &MPlayer::openFile);
    //播放暂停
    connect(ui->btnpp, &QAbstractButton::clicked, this, &MPlayer::togglePlayback);
    //拖动进度条调节播放位置
    //connect(ui->slipos, &QAbstractSlider::valueChanged, this, &MPlayer::setPosition); //实时调节播放位置，播放过程调节会有杂音,采用sliderrelease的信号，connect出错，只能在ui转到槽
    //connect(ui->slipos,&QAbstractSlider::sliderReleased,this, &MPlayer::setPosition);//编译不通过，转ui使用槽
    //从歌单中双击选取音乐进行播放，支持键盘上下选取回车播放
   // connect(ui->lwslist,&QListWidget::itemDoubleClicked,this,&MPlayer::lwsongchoose);//只能双击
    connect(ui->lwslist,&QListWidget::itemActivated,this,&MPlayer::lwsongchoose);//双击+回车
}

MPlayer::~MPlayer()
{
    delete mplayer; //初始化时设置了父类，其实不需要自己调用delete，但是以防万一嘛，也没有报错
    delete mplist;
    delete ui;
}

//获取音频文件的url
void MPlayer::openFile()
{
    QList<QUrl> urls= QFileDialog::getOpenFileUrls(this,tr("请选择要播放的音乐"),QUrl( "C:/"),"music(*.mp3 *.wav *.mpga)");
    if(urls.isEmpty())
    {
        qDebug()<<"empty urls";
    }
    else
    {
        playUrl(urls);
    }

}
//将url对应的音频添加到播放列表中
void MPlayer::playUrl(QList<QUrl>& urls)
{
    foreach(QUrl url,urls)
    {
        mplist->addMedia(url);
        //将歌单添加到listwidget
        ui->lwslist->addItem(QFileInfo(url.toLocalFile()).baseName());
        //qDebug()<<QFileInfo(url.toLocalFile()).baseName();
    }
    if(mplayer->state() != QMediaPlayer::PlayingState && mplayer->state() != QMediaPlayer::PausedState)
    {
        mplayer->play();
        ui->btnpp->setIcon(QIcon(":/image/image/pause.png"));
     }

}
//将QMedia的时间转换为常见的格式 分：秒
static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}

//使进度条随着播放自动滑动且更新当前播放位置信息
void MPlayer::updatePosition(qint64 position)
{
    ui->slipos->setValue(position);
    ui->labpos->setText(formatTime(position));
}

//更新进度条和范围和歌曲播放时长
void MPlayer::updateDuration(qint64 duration)
{
    ui->labdur->setText(formatTime(duration));
    ui->slipos->setRange(0,duration);
    ui->slipos->setEnabled(duration > 0);
    ui->slipos->setPageStep(duration / 10);
    updateInfo();
}

void MPlayer::setPosition(int position)
{
    if (qAbs(mplayer->position() - position) > 99) //变化时间太短99ms不管
        mplayer->setPosition(position);
}
//播放、暂停、未选取文件则选取文件
void MPlayer::togglePlayback()
{
    if (mplayer->mediaStatus() == QMediaPlayer::NoMedia)
    {
        openFile();
    }
    else if (mplayer->state() == QMediaPlayer::PlayingState)
    {
        mplayer->pause();
        ui->btnpp->setIcon(QIcon(":/image/image/play.png"));
    }
    else
    {
        mplayer->play();        
        ui->btnpp->setIcon(QIcon(":/image/image/pause.png"));
    }

}

////如果文件更名过就会解析信息错误,或者是经过转码的文件
//切歌时实时更换歌曲名和作家、歌曲封面图
void MPlayer::updateInfo()
{    
    QStringList info;
    QImage simg;
    if (mplayer->isMetaDataAvailable())
    {
        QString author = mplayer->metaData(QStringLiteral("Author")).toString();
        if (!author.isEmpty())
            info.append(author);
        QString title = mplayer->metaData(QStringLiteral("Title")).toString();
        if (!title.isEmpty())
            info.append(title);
        //获取封面图
        simg = mplayer->metaData(QStringLiteral("ThumbnailImage")).value<QImage>();

    }

    if(info.isEmpty())//有的文件没有足够信息提取到歌名
    {
        ui->labsname->setText("Kearney - You're the one");
    }
    else
    {
        ui->labsname->setText(info.join(tr(" - ")));
    }
    if(simg.isNull())  //有点音频文件被修改过没有封面图
    {
//        qDebug()<<"no image";
        ui->labsimag->setPixmap(QPixmap(":/image/image/designer.jpg"));
    }
    else
        ui->labsimag->setPixmap(QPixmap::fromImage(simg));


}

void MPlayer::handleError()
{
    const QString errorString = mplayer->errorString();
    this->ui->labsname->setText(errorString.isEmpty()
                       ? tr("Unknown error #%1").arg(int(mplayer->error()))
                       : tr("Error: %1").arg(errorString));
}

//下一首
void MPlayer::on_btnnext_clicked()
{
    mplist->next();
}
//上一首
void MPlayer::on_btnpre_clicked()
{
    mplist->previous();
}
//静音或解禁
void MPlayer::on_btnmute_clicked()
{
    static int tmpvol = mplayer->volume();
    ui->slivol->setValue(this->mplayer->volume());
    if(this->mplayer->isMuted())
    {
        ui->btnmute->setIcon(QIcon(":/image/image/volume.png"));
        this->mplayer->setMuted(false);
        ui->slivol->setValue(tmpvol);
    }
    else
    {
        ui->btnmute->setIcon(QIcon(":/image/image/mute.png"));
        this->mplayer->setMuted(true);
        ui->slivol->setValue(0);
    }

}

//音量条实时控制音量
void MPlayer::on_slivol_valueChanged(int value)
{
    this->mplayer->setVolume(value);
}

//在单曲循环、列表循环、单曲播放之间切换状态
void MPlayer::on_btnpmode_clicked()
{
    if(this->mplist->playbackMode() == QMediaPlaylist::Loop)    //由列表循环进入单曲循环
    {
        this->mplist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
//        this->ui->btnpmode->setText("单曲循环");
        this->ui->btnpmode->setIcon(QIcon(":/image/image/currentloop.png"));
    }
    else if(this->mplist->playbackMode() == QMediaPlaylist::CurrentItemInLoop)  //由单曲循环进入随机播放
    {
        this->mplist->setPlaybackMode(QMediaPlaylist::Random);
        this->ui->btnpmode->setIcon(QIcon(":/image/image/random.png"));
//        this->ui->btnpmode->setText("随机播放");
    }
    else if(this->mplist->playbackMode() == QMediaPlaylist::Random) //由随机播放进入顺序播放
    {
        this->mplist->setPlaybackMode(QMediaPlaylist::Sequential);
        this->ui->btnpmode->setIcon(QIcon(":/image/image/sequential.png"));
//        this->ui->btnpmode->setText("顺序播放");
    }
    else if(this->mplist->playbackMode() == QMediaPlaylist::Sequential) //由顺序播放进入列表循环
    {
        this->mplist->setPlaybackMode(QMediaPlaylist::Loop);
        this->ui->btnpmode->setIcon(QIcon(":/image/image/loop.png"));
//        this->ui->btnpmode->setText("列表循环");
    }
}

//如何在拖动未释放前就对文件进行筛选呢？？
void MPlayer::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
        qDebug("准备接收拖动文件");
    }
    else
    {
        event->ignore();
        qDebug("忽略拖动文件");
    }
}
void MPlayer::dropEvent(QDropEvent *event)
{
    const QMimeData *mimedata = event->mimeData();
    QList <QUrl> path = mimedata->urls();
    QMimeDatabase db;
    foreach(QUrl i,path )
    {
        QMimeType mime = db.mimeTypeForFile(i.toLocalFile());
        //格式工场转码后的wav文件测试是audio/x-wav
        if(mime.name()=="audio/mpeg" || mime.name()=="audio/wav" || mime.name()=="audio/x-wav")//格式是mp3\mpeg\wav就添加到播放列表
        {
//            qDebug()<<mime.name();
            mplist->addMedia(i);
            ui->lwslist->addItem(QFileInfo(i.toLocalFile()).baseName());
        }
        else
        {
            qDebug()<<mime.name();
            qDebug("----voi file ERROR！！！----not mpeg mp3 wav filetype");
        }
    }
    if(mplayer->state() != QMediaPlayer::PlayingState && mplayer->state() != QMediaPlayer::PausedState)
    {
        this->mplayer->play();
        ui->btnpp->setIcon(QIcon(":/image/image/play.png"));
    }

}
//从歌单中双击选取音乐进行播放，支持键盘上下选取回车播放
void MPlayer::lwsongchoose()
{
//    qDebug()<<ui->lwslist->currentRow();
    mplist->setCurrentIndex(ui->lwslist->currentRow());
    if(mplayer->state() != QMediaPlayer::PausedState || mplayer->state() == QMediaPlayer::PausedState)
    {
        this->mplayer->play();
        ui->btnpp->setIcon(QIcon(":/image/image/pause.png"));
    }
}

//拖动进度条调节播放位置
void MPlayer::on_slipos_sliderReleased()
{
    setPosition(ui->slipos->value());
}
// 进行最小化、最大化/还原、关闭操作
void MPlayer::on_btnquit_clicked()
{
    this->close();
}
void MPlayer::on_btnmax_clicked()
{
    this->isMaximized() ? this->showNormal() : this->showMaximized();
}
void MPlayer::on_btnmin_clicked()
{
    this->showMinimized();
}

//处理去掉标题栏无法移动的问题
void MPlayer::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)//仅对鼠标左键响应，右键留着使用菜单功能
        pos = e->globalPos();
}
void MPlayer::mouseMoveEvent(QMouseEvent *e)
{
    if (!e->buttons().testFlag(Qt::LeftButton))//仅对鼠标左键响应，右键留着使用菜单功能
            return;
    int dx = e->globalX() - pos.x();
    int dy = e->globalY() - pos.y();
    pos = e->globalPos();
    move(x()+dx, y()+dy);
}
