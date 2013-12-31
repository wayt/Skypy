#include <QBuffer>
#include <QByteArray>
#include <iostream>

#include "videosocket.h"

VideoSocket::VideoSocket(VideoViewer *videoViewer, QObject *parent) :
    QThread(parent),
    _socket(new QUdpSocket(this)),
    _hostAddr(),
    _hostPort(VIDEO_PORT),
    _peerAddr(),
    _peerPort(VIDEO_PORT),
    _run(false),
    _videoReader(new VideoReader(this)),
    _videoViewer(videoViewer)
{
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(_socket_readyRead()));
}

VideoSocket::~VideoSocket()
{
}

void VideoSocket::setHostAddr(const QHostAddress &addr, quint16 port)
{
    _hostAddr = addr;
    _hostPort = port;
    std::cout << "HOSTADDR: " << addr.toString().toStdString() << " - PORT: " << port << std::endl;
    _socket->bind(addr, port);
}

void VideoSocket::setPeerAddr(const QHostAddress& addr, quint16 port)
{
    _peerAddr = addr;
    _peerPort = port;
    std::cout << "PEERADDR: " << addr.toString().toStdString() << " - PORT: " << port << std::endl;
}

void VideoSocket::quit()
{
    _run = false;
    QThread::quit();
}

void VideoSocket::terminate()
{
    _run = false;
    QThread::terminate();
}

void VideoSocket::run()
{
    if (!_socket->isValid())
    {
        qDebug("Socket isn't valid");
        return ;
    }
    if (!_videoReader->isInitialized())
    {
        qDebug("No camera");
        return ;
    }

    _run = true;

    while (_run)
    {
        QImage img;

        if (!_videoReader->readFrame(img))
        {
            qDebug("Unable to read video frame");
            return ;
        }

        if (!_run)
            break;

        QByteArray ba;
        QBuffer buffer(&ba);
        img.save(&buffer, "JPG");
        QByteArray array = qCompress(ba, 9);

        qint64 size = _socket->writeDatagram(array, _peerAddr, _peerPort);
        qDebug("size write = %lld", size);
    }
}

void VideoSocket::_socket_readyRead()
{
    QByteArray data;

    while (_socket->hasPendingDatagrams())
    {
        data.resize(_socket->pendingDatagramSize());

        qint64 size = _socket->readDatagram(data.data(), data.size(), &_hostAddr, &_hostPort);
        qDebug("size read = %lld", size);
        QByteArray array = qUncompress(data);
        _videoViewer->setImage(array);
    }
}
