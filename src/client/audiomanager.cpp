#include "audioencoder.h"
#include "audiomanager.h"
#include <QTime>
#include <iostream>
#include "networkmgr.h"

AudioManager::AudioManager(QObject *parent) :
    QThread(parent),
    Singleton<AudioManager>(),
    _run(false),
    _errText(),
    _input(new AudioStream()),
    _output(new AudioStream()),
    _inputQueues(),
    _outputQueue(),
    _forwardQueues()
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

    _inputQueues.clear();
    _outputQueue.clear();

    _run = true;
    QThread::start();
    return true;
}

void AudioManager::terminate()
{
    if (!_run)
        return ;

    _run = false;
    QThread::terminate();

    wait();

    _input->stop();
    _output->stop();
    _inputQueues.clear();
}

void AudioManager::run()
{
    QTime timer;
    timer.start();
    AudioSample outSample;
    bool outSampleValid = false;

    while (_run)
    {
        EncodedSample encodedSample;
        AudioSample sample;

        //if (_input->inputQueue().isEmpty() && _output->outputQueue().isEmpty() && _forwardQueues.empty() && _run)
        //    QThread::msleep(10);

        if (!_outputQueue.isEmpty())
        {
            while (!_outputQueue.isEmpty())
            {
                AudioSample temp;
                encodedSample = _outputQueue.dequeue();
                if (sAudioEncoder->decode(temp, encodedSample))
                {
                    outSample += temp;
                    outSampleValid = true;
                }
            }
        }
        if (outSampleValid && timer.elapsed() >= 20)
        {
            std::cout << "ELAPSED: " << timer.elapsed() << " - SIZE: " << _output->outputQueue().size() << std::endl;
            timer.restart();
            _output->outputQueue().enqueue(outSample);
            outSample.clearBuffer();
            outSampleValid = false;
        }

        if (!_input->inputQueue().isEmpty())
        {
            std::cout << "INPUT NO EMPTY" << std::endl;
            sample = _input->inputQueue().dequeue();

            QMap<quint32, AudioSample> sampleMap;
            for (QMap<quint32, QQueue<EncodedSample>*>::ConstIterator itr = _forwardQueues.begin();
                 itr != _forwardQueues.end(); ++itr)
            {
                if (itr.value()->size() > 0 && sNetworkMgr->isAudioSocketConnect(itr.key()))
                {
                    AudioSample temp;
                    encodedSample = itr.value()->dequeue();
                    if (sAudioEncoder->decode(temp, encodedSample))
                    {
                        sampleMap[itr.key()] = temp;
                    }
                }
            }

            for (QMap<quint32, QSynchronizedQueue<EncodedSample>*>::Iterator itr = _inputQueues.begin();
                 itr != _inputQueues.end(); ++itr)
            {
                AudioSample sendSample = sample;

                for (QMap<quint32, AudioSample>::ConstIterator itr2 = sampleMap.begin();
                     itr2 != sampleMap.end(); ++itr2)
                    if (itr.key() != itr2.key())
                        sendSample += itr2.value();

                if (sAudioEncoder->encode(sendSample, encodedSample))
                    itr.value()->enqueue(encodedSample);
            }
        }
    }
}

void AudioManager::addInputPeer(quint32 id)
{
    _inputQueues[id] = new QSynchronizedQueue<EncodedSample>();
    _forwardQueues[id] = new QQueue<EncodedSample>();
}

void AudioManager::removeInputPeer(quint32 id)
{
    {
        QMap<quint32, QSynchronizedQueue<EncodedSample>*>::Iterator itr = _inputQueues.find(id);
        if (itr != _inputQueues.end())
        {
            QSynchronizedQueue<EncodedSample>* input = itr.value();
            _inputQueues.erase(itr);
            delete input;
        }
    }

    {
        QMap<quint32, QQueue<EncodedSample>*>::Iterator itr = _forwardQueues.find(id);
        if (itr != _forwardQueues.end())
        {
            QQueue<EncodedSample>* input = itr.value();
            _forwardQueues.erase(itr);
            delete input;
        }
    }
}

void AudioManager::addToForwardEncodedSample(EncodedSample const& data, quint32 peerId)
{
    _forwardQueues[peerId]->enqueue(data);
}
