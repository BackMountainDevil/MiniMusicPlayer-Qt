#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QMainWindow>
#include <QMediaPlayer>
namespace Ui {
class ListWidget;
}

class ListWidget : public QMainWindow
{
    Q_OBJECT

public:
    explicit ListWidget(QWidget *parent = nullptr);
    ~ListWidget();
    void additem(QString str); //将歌曲添加到默认歌单

private:
    Ui::ListWidget *ui;
};

#endif // LISTWIDGET_H
