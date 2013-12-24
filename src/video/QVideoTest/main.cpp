#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w(QString(argv[1]).toInt());
    w.show();

    return a.exec();
}
