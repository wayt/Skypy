#include "audiostream.h"

int AudioStream::_nbStream = 0;

AudioStream::AudioStream() :
    _streamType(NO_STREAM),
    _errText(),
    _inputParameter(),
    _outputParameter(),
    _inputDeviceInfo(NULL),
    _outputDeviceInfo(NULL),
    _stream(NULL),
    _latency(LOW_LATENCY),
    _frequency(AudioSample::FREQ_8000),
    _channelCount(0),
    _isOpen(false),
    _isActive(false),
    _inputQueue(),
    _outputQueue()
{
    if (_nbStream == 0)
        Pa_Initialize();

    ++_nbStream;
}

AudioStream::~AudioStream()
{
    stop();
    closeDevice();

    --_nbStream;
    if (_nbStream == 0)
        Pa_Terminate();
}

bool AudioStream::setInputDevice(int device, AudioSample::eChannel channel, eLatency latency)
{
    if (device < 0)
        device = Pa_GetDefaultInputDevice();

    return _setDevice(device, channel, latency, _inputParameter, INPUT_STREAM, _inputDeviceInfo);
}

bool AudioStream::setOutputDevice(int device, AudioSample::eChannel channel, eLatency latency)
{
    if (device < 0)
        device = Pa_GetDefaultOutputDevice();

    return _setDevice(device, channel, latency, _outputParameter, OUTPUT_STREAM, _outputDeviceInfo);
}

bool AudioStream::openDevice(AudioSample::eFrequency frequency)
{
    PaError err;
    PaStreamParameters *input = (_streamType & INPUT_STREAM) ? &_inputParameter : NULL;
    PaStreamParameters *output = (_streamType & OUTPUT_STREAM) ? &_outputParameter : NULL;
    double sample = AudioSample::frequencyToDouble(frequency);

    if (_isOpen)
        closeDevice();

    if (_channelCount == 0)
    {
        _errText = "The input or the output must be set before openning";
        return false;
    }

    if (qFuzzyCompare(sample, 0.))
    {
        _errText = "Invalid frequency choose";
        return false;
    }

    if ((err = Pa_OpenStream(&_stream, input, output, sample, NB_FRAMES_PER_BUFFER * _channelCount, paClipOff, &AudioStream::_callback, this)) != paNoError)
    {
        _errText = Pa_GetErrorText(err);
        return false;
    }

    _frequency = frequency;
    _isOpen = true;
    return true;
}

void AudioStream::closeDevice()
{
    if (!_isOpen)
        return ;

    if (_isActive)
        stop();

    Pa_CloseStream(_stream);
    _isOpen = false;
}

bool AudioStream::start()
{
    PaError err;

    if (_isActive)
    {
        _errText = "Stream is already active";
        return false;
    }

    if ((err = Pa_StartStream(_stream)) != paNoError)
    {
        _errText = Pa_GetErrorText(err);
        return false;
    }

    _inputQueue.clear();
    _outputQueue.clear();

    _isActive = true;
    return true;
}

void AudioStream::stop()
{
    if (!_isActive)
        return ;

    Pa_StopStream(_stream);
    _isActive = false;
}

int AudioStream::deviceCount()
{
    return Pa_GetDeviceCount();
}

const QString &AudioStream::deviceName(int device)
{
    static QString name;
    const PaDeviceInfo* info = Pa_GetDeviceInfo(device);

    if (!info)
        name.clear();
    else
        name = info->name;

    return name;
}

bool AudioStream::_setDevice(int device, AudioSample::eChannel channel, eLatency latency, PaStreamParameters &parameter, eStreamTypes streamType, const PaDeviceInfo *&info)
{
    int channelCount = AudioSample::channelToInt(channel);

    if (device == paNoDevice)
    {
        _errText = "No default device found";
        return false;
    }

    if ((info = Pa_GetDeviceInfo(device)) == NULL)
    {
        _errText = "Invalid device";
        return false;
    }

    if (channelCount == 0)
    {
        _errText = "Incorrect number of channel";
        return false;
    }

    parameter.device = device;
    parameter.channelCount = channelCount;
    parameter.hostApiSpecificStreamInfo = NULL;
    parameter.sampleFormat = SAMPLE_FORMAT;
    switch (latency)
    {
        case LOW_LATENCY:
            parameter.suggestedLatency = (streamType == INPUT_STREAM) ? info->defaultLowInputLatency : info->defaultLowOutputLatency;
            break;
        case HIGH_LATENCY:
            parameter.suggestedLatency = (streamType == INPUT_STREAM) ? info->defaultHighInputLatency : info->defaultHighOutputLatency;
            break;
        default:
            _errText = "Incorrect latency";
            return false;
    }

    _latency = latency;
    _channel = channel;
    _channelCount = channelCount;
    _streamType |= streamType;
    return true;
}

void AudioStream::_clearDevice(PaStreamParameters &parameter, const PaDeviceInfo *&info)
{
    info = NULL;
    parameter.device = paNoDevice;
    parameter.channelCount = 0;
    parameter.hostApiSpecificStreamInfo = NULL;
    parameter.sampleFormat = 0;
    parameter.suggestedLatency = 0;
}

/**********************************************/
/* This function must be the fastest possible */
/**********************************************/
int AudioStream::_callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    AudioSample sample;
    AudioStream *t = reinterpret_cast<AudioStream*>(userData);

    Q_UNUSED(timeInfo);
    Q_UNUSED(statusFlags);

    if (input && (t->_streamType & INPUT_STREAM))
    {
        sample.setBuffer(static_cast<const SAMPLE*>(input), frameCount);
        t->_inputQueue.enqueue(sample);
    }

    if (!(t->_streamType & OUTPUT_STREAM) || !output || t->_outputQueue.size() == 0)
    {
        if (output)
            memset(output, 0, sizeof(SAMPLE) * frameCount);
        return paContinue;
    }

    SAMPLE *out = static_cast<SAMPLE*>(output);
    sample = t->_outputQueue.dequeue();
    for (int i = 0; i < sample.nbFrame(); ++i)
        out[i] = sample[i];

    return paContinue;
}
