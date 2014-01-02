#include "audioencoder.h"
#include "audiomanager.h"

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

    _input->stop();
    _output->stop();
    _run = false;
    _inputQueues.clear();
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

            QMap<quint32, AudioSample> sampleMap;
            for (QMap<quint32, QQueue<EncodedSample>*>::ConstIterator itr2 = _forwardQueues.begin();
                 itr2 != _forwardQueues.end(); ++itr2)
            {
                if (itr2.value()->size() > 0)
                    sAudioEncoder->decode(sampleMap[itr2.key()], itr2.value()->dequeue());
            }

            AudioSample saveSample = sample;
            for (QMap<quint32, QSynchronizedQueue<EncodedSample>*>::Iterator itr = _inputQueues.begin();
                 itr != _inputQueues.end(); ++itr)
            {
                sample = saveSample;

                for (QMap<quint32, AudioSample>::ConstIterator itr2 = sampleMap.begin();
                     itr2 != sampleMap.end(); ++itr2)
                    if (itr.key() != itr2.key())
                        sample += itr2.value();

                if (sAudioEncoder->encode(sample, encodedSample))
                {
                    itr.value()->enqueue(encodedSample);
                }
            }
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
