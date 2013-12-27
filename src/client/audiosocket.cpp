#include "audiomanager.h"
#include "audiosocket.h"
#include <iostream>

AudioSocket::AudioSocket(QObject *parent) :
    QThread(parent),
    _run(false),
    _socket(new QUdpSocket(this)),
    _hostAddr(),
    _hostPort(AUDIO_PORT)
{
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(_socket_readyRead()));
}

AudioSocket::~AudioSocket()
{
}

bool AudioSocket::setHostAddr(const QHostAddress &addr, quint16 port)
{
    _hostAddr = addr;
    _hostPort = port;
    std::cout << "HOSTADDR: " << addr.toString().toStdString() << " - PORT: " << port << std::endl;
    if (_socket->isOpen())
        _socket->close();
    if (!_socket->bind(addr, port))
    {
        std::cout << "setHostAddr: " << _socket->errorString().toStdString() << std::endl;
        return false;
    }
    return true;
}

void AudioSocket::setPeerAddr(const QHostAddress& addr, quint16 port)
{
    _peerAddr = addr;
    _peerPort = port;
    std::cout << "PEERADDR: " << addr.toString().toStdString() << " - PORT: " << port << std::endl;
}

void AudioSocket::quit()
{
    _run = false;
    QThread::quit();
    if (_socket->isOpen())
        _socket->close();
}

void AudioSocket::terminate()
{
    _run = false;
    QThread::terminate();
}

void AudioSocket::run()
{
    if (!_socket->isValid())
    {
        qDebug("Socket isn't valid");
        return ;
    }

    _run = true;

    while (_run)
    {
        while ((!sAudioManager->input()->isStarted() || !sAudioManager->output()->isStarted()) && _run)
            QThread::msleep(100);

        if (!_run)
            break;

        EncodedSample encodedSample = sAudioManager->inputQueue().dequeue();
        //std::cout << "WRITE AUDIO ON: " << _peerAddr.toString().toStdString() << ":" << _peerPort << std::endl;
        _socket->writeDatagram(encodedSample.encodedSample(), _peerAddr, _peerPort);
    }
    if (_socket->isOpen())
        _socket->close();
}

void AudioSocket::_socket_readyRead()
{
    QByteArray data;

    while (_socket->hasPendingDatagrams())
    {
        data.resize(_socket->pendingDatagramSize());

        _socket->readDatagram(data.data(), data.size(), &_hostAddr, &_hostPort);
        sAudioManager->push(EncodedSample(data));
    }
}
