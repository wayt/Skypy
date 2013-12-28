#include "audioencoder.h"
#include "audiomanager.h"

AudioManager::AudioManager(QObject *parent) :
    QThread(parent),
    Singleton<AudioManager>(),
    _run(false),
    _errText(),
    _input(new AudioStream()),
    _output(new AudioStream()),
    _inputQueue(),
    _outputQueue()
{
}

AudioManager::~AudioManager()
{
    delete _input;
    delete _output;
}

bool AudioManager::setInputDevice(int device, AudioSample::eChannel channel, AudioSample::eFrequency frequency)
{
    if (_input->setInputDevice(device, channel))
        if (_input->openDevice(frequency))
            return true;

    _errText = _input->errorText();
    emit error(_errText);
    return false;
}

bool AudioManager::setOutputDevice(int device, AudioSample::eChannel channel, AudioSample::eFrequency frequency)
{
    if (_output->setOutputDevice(device, channel))
        if (_output->openDevice(frequency))
            return true;

    _errText = _output->errorText();
    emit error(_errText);
    return false;
}

bool AudioManager::start()
{
    if (!_input->isStarted() && !_input->start())
    {
        _errText = _input->errorText();
        emit error(_errText);
        return false;
    }
    if (!_output->isStarted() && !_output->start())
    {
        _errText = _output->errorText();
        emit error(_errText);
        return false;
    }
    if (!sAudioEncoder->isInitialized())
    {
        if (!sAudioEncoder->init(_input->frequency(), _input->channel()))
        {
            _errText = sAudioEncoder->errorText();
            emit error(_errText);
            return false;
        }
    }

    _run = true;
    QThread::start();
    return true;
}

void AudioManager::quit()
{
    if (!_run)
        return;

    _input->stop();
    _output->stop();
    _run = false;
    QThread::quit();
}

void AudioManager::terminate()
{
    if (!_run)
        return ;

    _input->stop();
    _output->stop();
    _run = false;
    QThread::terminate();
}

void AudioManager::run()
{
    while (_run)
    {
        AudioSample sample;
        EncodedSample encodedSample;

        if (_input->inputQueue().isEmpty() && _output->outputQueue().isEmpty())
            QThread::msleep(10);

        // Audio stream (raw data) to input queue (Encoded data)
        while (!_input->inputQueue().isEmpty())
        {
            sample = _input->inputQueue().dequeue();
            if (sAudioEncoder->encode(sample, encodedSample))
                _inputQueue.enqueue(encodedSample);
        }

        // Output queue (Encoded data) to audio stream (raw data)
        while (!_outputQueue.isEmpty())
        {
            encodedSample = _outputQueue.dequeue();
            if (sAudioEncoder->decode(sample, encodedSample))
                _output->outputQueue().enqueue(sample);
        }
    }
}
