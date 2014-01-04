#include <QtGlobal>

#include "audiosample.h"
#include <iostream>
#include <cstring>

AudioSample::AudioSample() :
    _nbFrame(0),
    _buffer()
{
    for (int i = 0; i < NB_MAX_FRAMES; ++i)
        _buffer[i] = ZERO;
}

AudioSample::AudioSample(const SAMPLE *buffer, int nbFrame) :
    _nbFrame(0),
    _buffer()
{
    int maxFrame = qMin(nbFrame, NB_MAX_FRAMES);

    memcpy(_buffer, buffer, maxFrame * sizeof(SAMPLE));
    _nbFrame = maxFrame;
}

AudioSample::AudioSample(const AudioSample &other) :
    _nbFrame(other._nbFrame),
    _buffer()
{
    memcpy(_buffer, other._buffer, _nbFrame * sizeof(SAMPLE));
}

AudioSample::~AudioSample()
{
}

AudioSample& AudioSample::operator=(const AudioSample &other)
{
    _nbFrame = other._nbFrame;

    memcpy(_buffer, other._buffer, _nbFrame * sizeof(SAMPLE));

    return *this;
}

AudioSample& AudioSample::operator+=(const AudioSample &other)
{
    int nbFrameMax = qMax(_nbFrame, other._nbFrame);

    for (int i = 0; i < nbFrameMax; ++i)
        _buffer[i] = (_buffer[i] + other[i]) - ((_buffer[i] * other[i]) / LIMIT_VALUE);
    _nbFrame = nbFrameMax;

    return *this;
}

void AudioSample::setBuffer(const SAMPLE *buffer, int nbFrame)
{
    int maxFrame = qMin(nbFrame, NB_MAX_FRAMES);

    memcpy(_buffer, buffer, maxFrame * sizeof(SAMPLE));
    _nbFrame = maxFrame;
}

void AudioSample::clearBuffer()
{
    for (int i = 0; i < NB_MAX_FRAMES; ++i)
        _buffer[i] = ZERO;
    _nbFrame = 0;
}

void AudioSample::dumpBuffer() const
{
    std::cout << "BUFFER: " << sizeof(SAMPLE) << " - FRAMES: " << _nbFrame << std::endl;
    for (int i = 0; i < _nbFrame; ++i)
    {
        if (i % 10 == 0)
            std::cout << std::endl;
        std::cout << _buffer[i];
    }
    std::cout << std::endl;
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
