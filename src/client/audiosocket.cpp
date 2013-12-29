#include "audiomanager.h"
#include "audiosocket.h"
#include <iostream>
#include <QDateTime>
#include "networkmgr.h"

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
    if (_socket->state() != QAbstractSocket::UnconnectedState)
    {
        std::cout << "ABORT OPENED SOCKET" << std::endl;
        _socket->abort();
        _socket->close();
    }
    if (!_socket->bind(QHostAddress::Any, port))
    {
        std::cout << "setHostAddr: " << _socket->errorString().toStdString() << " (" << _socket->error() << ")" << std::endl;
        return false;
    }
    _inputReaded = false;
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
    _socket->abort();
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

    QTime inputTimer;
    inputTimer.start();

    while (_run)
    {
        while ((!sAudioManager->input()->isStarted() || !sAudioManager->output()->isStarted()) && _run)
            QThread::msleep(100);

        if (!_run)
            break;

        if (!_inputReaded)
        {
            int mstime = inputTimer.elapsed();
            if (mstime > 2000)
            {
                sNetworkMgr->handleAudioNoInput();
                inputTimer.restart();
            }

        }

        EncodedSample encodedSample = sAudioManager->inputQueue().dequeue();
        //std::cout << "WRITE AUDIO ON: " << _peerAddr.toString().toStdString() << ":" << _peerPort << std::endl;
        _socket->writeDatagram(encodedSample.encodedSample(), _peerAddr, _peerPort);
    }
    _socket->abort();
    _socket->close();
}

void AudioSocket::_socket_readyRead()
{
    QByteArray data;

    while (_socket->hasPendingDatagrams())
    {
        data.resize(_socket->pendingDatagramSize());

        QHostAddress fromAddr;
        quint16 fromPort;

        _socket->readDatagram(data.data(), data.size(), &fromAddr, &fromPort);
        //std::cout << "AUDIO READ FROM: " << fromAddr.toString().toStdString() << ":" << fromPort << std::endl;
        if (fromAddr == _peerAddr && fromPort == _peerPort)
        {
            _inputReaded = true;
            sAudioManager->push(EncodedSample(data));
        }
    }
}
