#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <QtGlobal>
#include <QString>
#include <portaudio.h>

#include "audiosample.h"
#include "qsynchronizedqueue.h"

class AudioStream
{
public:
    enum eStreamType
    {
        NO_STREAM       = 0x00,
        INPUT_STREAM    = 0x01,
        OUTPUT_STREAM   = 0x02,
        IO_STREAM       = INPUT_STREAM | OUTPUT_STREAM
    };
    Q_DECLARE_FLAGS(eStreamTypes, eStreamType)

    enum eLatency
    {
        LOW_LATENCY     = 0,
        HIGH_LATENCY    = 1
    };

public:
    AudioStream();
    virtual ~AudioStream();

    bool setInputDevice(int device = -1, AudioSample::eChannel channel = AudioSample::MONO, eLatency latency = LOW_LATENCY);
    bool setOutputDevice(int device = -1, AudioSample::eChannel channel = AudioSample::MONO, eLatency latency = LOW_LATENCY);
    inline void clearInputDevice() { _clearDevice(_inputParameter, _inputDeviceInfo); }
    inline void clearOutputDevice() { _clearDevice(_outputParameter, _outputDeviceInfo); }

    inline eStreamTypes streamType() const { return _streamType; }

    bool openDevice(AudioSample::eFrequency frequency = AudioSample::FREQ_48000);
    void closeDevice();

    inline bool isOpen() const { return _isOpen; }

    bool start();
    void stop();

    inline bool isStarted() const { return _isActive; }

    inline QSynchronizedQueue<AudioSample>& inputQueue() { return _inputQueue; }
    inline const QSynchronizedQueue<AudioSample>& inputQueue() const { return _inputQueue; }
    inline QSynchronizedQueue<AudioSample>& outputQueue() { return _outputQueue; }
    inline const QSynchronizedQueue<AudioSample>& outputQueue() const { return _outputQueue; }

    inline QString getErrorText() const { return _errText; }

private:
    bool _setDevice(int device, AudioSample::eChannel channel, eLatency latency, PaStreamParameters &parameter, eStreamTypes streamType, const PaDeviceInfo *&info);
    void _clearDevice(PaStreamParameters &parameter, const PaDeviceInfo *&info);

    static int _callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

private:
    eStreamTypes _streamType;
    QString _errText;
    PaStreamParameters _inputParameter;
    PaStreamParameters _outputParameter;
    const PaDeviceInfo *_inputDeviceInfo;
    const PaDeviceInfo *_outputDeviceInfo;
    PaStream *_stream;
    bool _isOpen;
    bool _isActive;

    QSynchronizedQueue<AudioSample> _inputQueue;
    QSynchronizedQueue<AudioSample> _outputQueue;

private:
    static int _nbStream;

private:
    friend int _callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AudioStream::eStreamTypes)

#endif // AUDIOSTREAM_H
