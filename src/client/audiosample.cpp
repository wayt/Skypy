#include <QtGlobal>

#include "audiosample.h"

AudioSample::AudioSample() :
    _nbFrame(0),
    _buffer()
{
}

AudioSample::AudioSample(const SAMPLE *buffer, int nbFrame) :
    _nbFrame(0),
    _buffer()
{
    int maxFrame = qMin(nbFrame, NB_MAX_FRAMES);

    for (int i = 0; i < maxFrame; ++i)
        _buffer[i] = buffer[i];
    _nbFrame = maxFrame;
}

AudioSample::AudioSample(const AudioSample &other) :
    _nbFrame(other._nbFrame),
    _buffer()
{
    for (int i = 0; i < _nbFrame; ++i)
        _buffer[i] = other._buffer[i];
}

AudioSample::~AudioSample()
{
}

AudioSample& AudioSample::operator=(const AudioSample &other)
{
    _nbFrame = other._nbFrame;

    for (int i = 0; i < _nbFrame; ++i)
        _buffer[i] = other._buffer[i];

    return *this;
}

AudioSample& AudioSample::operator+=(const AudioSample &other)
{
    for (int i = 0; i < other._nbFrame; ++i)
    {
        _buffer[i] = _buffer[i] + other[i];
        if (_buffer[i] < MIN_VALUE)
            _buffer[i] = MIN_VALUE;
        else if (_buffer[i] > MAX_VALUE)
            _buffer[i] = MAX_VALUE;
    }

    return *this;
}

void AudioSample::setBuffer(const SAMPLE *buffer, int nbFrame)
{
    int maxFrame = qMin(nbFrame, NB_MAX_FRAMES);

    for (int i = 0; i < maxFrame; ++i)
        _buffer[i] = buffer[i];
    _nbFrame = maxFrame;
}

void AudioSample::clearBuffer()
{
    for (int i = 0; i < NB_MAX_FRAMES; ++i)
        _buffer[i] = ZERO;
    _nbFrame = 0;
}

double AudioSample::frequencyToDouble(eFrequency frequency)
{
    switch (frequency)
    {
        case FREQ_8000: return 8000.;
        case FREQ_12000: return 12000.;
        case FREQ_16000: return 16000.;
        case FREQ_24000: return 24000.;
        case FREQ_48000: return 48000.;
        default: return 0.;
    }
}

int AudioSample::channelToInt(eChannel channel)
{
    switch (channel)
    {
        case MONO: return 1;
        case STEREO: return 2;
        default: return 0;
    }
}
