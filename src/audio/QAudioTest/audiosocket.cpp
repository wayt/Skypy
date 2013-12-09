#include "audiomanager.h"
#include "audiosocket.h"

AudioSocket::AudioSocket(QObject *parent) :
    QThread(parent),
    _run(false),
    _socket(new QUdpSocket(this)),
    _hostAddr()
{
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(_socket_readyRead()));
}

AudioSocket::~AudioSocket()
{
}

void AudioSocket::setHostAddr(const QHostAddress &addr)
{
    _hostAddr = addr;
    _socket->bind(addr, AUDIO_PORT);
}

void AudioSocket::quit()
{
    _run = false;
    QThread::quit();
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
        _socket->writeDatagram(encodedSample.encodedSample(), _hostAddr, AUDIO_PORT);
    }
}

void AudioSocket::_socket_readyRead()
{
    QByteArray data;
    quint16 port = AUDIO_PORT;

    while (_socket->hasPendingDatagrams())
    {
        data.resize(_socket->pendingDatagramSize());

        _socket->readDatagram(data.data(), data.size(), &_hostAddr, &port);
        sAudioManager->push(EncodedSample(data));
    }
}
