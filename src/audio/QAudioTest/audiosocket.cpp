#include "audiosocket.h"

AudioSocket::AudioSocket(QObject *parent) :
    QThread(parent),
    _run(false),
    _socket(new QUdpSocket(this)),
    _hostAddr(),
    _input(NULL),
    _output(NULL),
    _encodedSample(new EncodedSample())
{
    QObject::connect(_socket, SIGNAL(readyRead()), this, SLOT(_socket_readyRead()));
}

AudioSocket::~AudioSocket()
{
    delete _encodedSample;
}

void AudioSocket::setHostAddr(const QHostAddress &addr)
{
    _hostAddr = addr;
    _socket->bind(addr, AUDIO_PORT);
}

void AudioSocket::setGain(int gain)
{
    _encodedSample->setGain(gain);
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
    if (!_input)
    {
        qDebug("Input is not set");
        return ;
    }

    if (!_encodedSample->init(AudioSample::FREQ_48000, AudioSample::MONO))
    {
        qDebug("Fail to init encoder/decoder: %s", qPrintable(_encodedSample->errText()));
        return ;
    }

    _run = true;

    while (_run)
    {
        while ((!_input->isStarted() || !_output->isStarted()) && _run)
            QThread::msleep(100);

        if (!_run)
            break;

        AudioSample sample = _input->inputQueue().dequeue();
        if (!_encodedSample->encode(sample))
            continue;

        qDebug("send. encoded sample size = %d, sample size = %d", _encodedSample->size(), sample.nbFrame());

        _socket->writeDatagram(_encodedSample->encodedSample(), _hostAddr, AUDIO_PORT);
    }
}

void AudioSocket::_socket_readyRead()
{
    QByteArray data;
    quint16 port = AUDIO_PORT;

    if (!_output)
    {
        qDebug("Output is not set");
        return ;
    }

    while (_socket->hasPendingDatagrams())
    {
        data.resize(_socket->pendingDatagramSize());

        _socket->readDatagram(data.data(), data.size(), &_hostAddr, &port);

        AudioSample sample;
        _encodedSample->setEncodedSample(data);
        if (!_encodedSample->decode(sample))
            continue;

        qDebug("receive. encoded sample size = %d, sample size = %d", _encodedSample->size(), sample.nbFrame());

        _output->outputQueue().enqueue(sample);
    }
}
