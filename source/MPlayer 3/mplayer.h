#ifndef MPLAYER_H
#define MPLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>

QT_BEGIN_NAMESPACE
namespace Ui { class MPlayer; }
QT_END_NAMESPACE

class MPlayer : public QWidget
{
    Q_OBJECT

public:
    MPlayer(QWidget *parent = nullptr);
    ~MPlayer();
    void dragEnterEvent(QDragEnterEvent *event) ;
    void dropEvent(QDropEvent *event) ;

public slots:
    void openFile();//获取音频文件的url
    void playUrl(QList<QUrl>& urls);//将url对应的音频添加到播放列表中
    void togglePlayback();//播放、暂停、未选取文件则选取文件
    void lwsongchoose();//从歌单中双击选取音乐进行播放，支持键盘上下选取回车播放

protected:
    void mousePressEvent(QMouseEvent *e);//处理去掉标题栏无法移动的问题
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void updatePosition(qint64 position);//使进度条随着播放自动滑动且更新当前播放位置信息
    void updateDuration(qint64 duration);//更新进度条的范围和歌曲总播放时长
    void setPosition(int position);
    void updateInfo();//切歌时实时更换歌曲名和作家
    void handleError();
    void on_btnnext_clicked();//下一首
    void on_btnpre_clicked();//上一首
    void on_btnmute_clicked();//静音或解禁
    void on_slivol_valueChanged(int value);//音量条实时控制音量
    void on_btnpmode_clicked();//在单曲循环、列表循环、单曲播放之间切换状态
    void on_slipos_sliderReleased();//拖动进度条调节播放位置、无过程音
    void on_btnquit_clicked();// 自定义标题栏进行进行最小化、最大化/还原、关闭操作
    void on_btnmax_clicked();
    void on_btnmin_clicked();

private:
    Ui::MPlayer *ui;
    QMediaPlayer *mplayer;  //播放器指针
    QMediaPlaylist *mplist; //播放列表指针
    QPoint pos;  //鼠标相对于窗口的位置，不是相对屏幕的位置

};
#endif // MPLAYER_H
