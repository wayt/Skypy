#include <QApplication>
#include <QDesktopWidget>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    w.move(qApp->desktop()->width() / 2 - w.width() / 2, qApp->desktop()->height() / 2 - w.height() / 2);

    return a.exec();
}
