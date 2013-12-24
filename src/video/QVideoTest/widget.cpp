#include <QMessageBox>

#include "widget.h"

Widget::Widget(int id, QWidget *parent) :
    QWidget(parent),
    _leIp(new QLineEdit(this)),
    _pbStart(new QPushButton("Start", this)),
    _layV(new QVBoxLayout(this)),
    _videoViewer(new VideoViewer(this)),
    _videoSocket(new VideoSocket(_videoViewer, this)),
    _id(id)
{
    _layV->addWidget(_leIp);
    _layV->addWidget(_pbStart);
    _layV->addWidget(_videoViewer, 1);

    _leIp->setInputMask("000.000.000.000;_");
    _leIp->setText("127.000.000.001");

    _videoViewer->setMinimumSize(640, 480);

    QObject::connect(_pbStart, SIGNAL(clicked()), this, SLOT(_pbStart_clicked()));
}

Widget::~Widget()
{
    _videoSocket->quit();
    _videoSocket->wait();
}

void Widget::_pbStart_clicked()
{
    _videoSocket->setHostAddr(QHostAddress::LocalHost, VIDEO_PORT + ((_id == 0) ? 0 : 1));
    _videoSocket->setPeerAddr(QHostAddress(_leIp->text()), VIDEO_PORT + ((_id == 0) ? 1 : 0));

    _videoSocket->start();

    _pbStart->setEnabled(false);
}
