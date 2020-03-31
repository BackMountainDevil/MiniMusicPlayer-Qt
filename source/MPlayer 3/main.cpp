#include "mplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    a.setFont();
    MPlayer w;
    w.show();
    return a.exec();
}
