#include "playaudio.h"
#include "encodedsample.h"

PlayAudio::PlayAudio(QObject *parent) :
    QThread(parent),
    _input(NULL),
    _output(NULL),
    _encodedSample(new EncodedSample()),
    _run(false)
{
}

PlayAudio::~PlayAudio()
{
    delete _encodedSample;
}

void PlayAudio::quit()
{
    _run = false;
    QThread::quit();
}

void PlayAudio::terminate()
{
    _run = false;
    QThread::terminate();
}

void PlayAudio::setGain(int gain)
{
    _encodedSample->setGain(gain);
}

void PlayAudio::run()
{
    if (!_input || !_output)
        return ;

    if (!_encodedSample->init(AudioSample::FREQ_48000, AudioSample::MONO))
    {
        qDebug("Canno't init encoder/decoder: %s", qPrintable(_encodedSample->errText()));
        return ;
    }

    _run = true;

    while (_run)
    {
        while ((!_input->isStarted() || !_output->isStarted()) && _run)
            QThread::msleep(100);

        if (!_run)
            break;

        AudioSample sample(_input->inputQueue().dequeue());
        _encodedSample->encode(sample);

        _encodedSample->decode(sample);
        _output->outputQueue().enqueue(sample);
    }
}
