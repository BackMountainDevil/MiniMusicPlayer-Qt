#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QSound>
#include <QSoundEffect>
#include <QMediaPlayer>
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

    void on_pushButton_4_clicked(); //一个按键实现播放暂停两个操作
    void on_pushButton_clicked();   //选取文件

private:
    Ui::MainWindow *ui;
    QMediaPlayer *voi;  //要播放的文件指针
    bool isplay = false;    //判断按键的状态
};
#endif // MAINWINDOW_H
