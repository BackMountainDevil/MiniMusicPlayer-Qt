#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include "listwidget.h"
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//播放器界面
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initplayer();  //界面和逻辑初始化设置
    void voiplay(); //实现音乐和按钮转换
    void voistop(); //实现暂停和按钮转换
    void dragEnterEvent(QDragEnterEvent *event);    //接收拖进来的音乐文件
    void dropEvent(QDropEvent *event);  //解析拖进来的文件并播放

protected:
//    void mouseMoveEvent(QMouseEvent *event); //鼠标拖动窗体时隐藏歌单，拖动完成后在显示
//    void mousePressEvent(QMouseEvent *event);
//    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);    //鼠标滚轮上下滚动实现音量调节

private slots:    
    void on_pushButton_4_clicked(); //一个按键实现播放暂停两个操作
    void on_pushButton_clicked();   //打开对话框选取特定的音乐文件并播放
    void on_pushButton_2_clicked(); //点击按钮后显示/隐藏播放列表
    void on_toolButton_clicked();   //清空播放列表和歌单窗口

private:
    Ui::MainWindow *ui;
    QMediaPlayer *voi;  //播放器指针
    QMediaPlaylist *voilist;    //播放菜单指针    
    ListWidget *list;    //歌单窗口
    int volume = 70;   //初始音量为70%
//按键用ui生成了这里不显示，布局也是用ui布局

};
#endif // MAINWINDOW_H
