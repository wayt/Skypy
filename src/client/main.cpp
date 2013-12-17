#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    w.resize(250, 500);
    w.show();

    return a.exec();
}
