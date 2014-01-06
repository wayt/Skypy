#ifndef AUDIOSTREAM_H
#define AUDIOSTREAM_H

#include <QtGlobal>
#include <QString>
#include <portaudio.h>

#include "audiosample.h"
#include "qsynchronizedqueue.h"

#define DEFAULT_DEVICE  -1

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

    bool setInputDevice(int device = DEFAULT_DEVICE, AudioSample::eChannel channel = AudioSample::MONO, eLatency latency = LOW_LATENCY);
    bool setOutputDevice(int device = DEFAULT_DEVICE, AudioSample::eChannel channel = AudioSample::MONO, eLatency latency = LOW_LATENCY);
    inline void clearInputDevice() { _clearDevice(_inputParameter, _inputDeviceInfo); }
    inline void clearOutputDevice() { _clearDevice(_outputParameter, _outputDeviceInfo); }

    inline int inputDevice() const { return _inputDevice; }
    inline int outputDevice() const { return _outputDevice; }

    inline eStreamTypes streamType() const { return _streamType; }

    bool openDevice(AudioSample::eFrequency frequency = AudioSample::FREQ_48000);
    void closeDevice();

    inline eLatency latency() const { return _latency; }
    inline AudioSample::eFrequency frequency() const { return _frequency; }
    inline AudioSample::eChannel channel() const { return _channel; }

    inline bool isOpen() const { return _isOpen; }

    bool start();
    void stop();

    inline bool isStarted() const { return _isActive; }

    inline QSynchronizedQueue<AudioSample>& inputQueue() { return _inputQueue; }
    inline const QSynchronizedQueue<AudioSample>& inputQueue() const { return _inputQueue; }
    inline QSynchronizedQueue<AudioSample>& outputQueue() { return _outputQueue; }
    inline const QSynchronizedQueue<AudioSample>& outputQueue() const { return _outputQueue; }

    inline const QString& errorText() const { return _errText; }

public:
    static int deviceCount();
    static const QString& deviceName(int device);

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
    eLatency _latency;
    AudioSample::eFrequency _frequency;
    AudioSample::eChannel _channel;
    int _channelCount;
    bool _isOpen;
    bool _isActive;
    int _inputDevice;
    int _outputDevice;

    QSynchronizedQueue<AudioSample> _inputQueue;
    QSynchronizedQueue<AudioSample> _outputQueue;

private:
    static int _nbStream;

private:
    friend int _callback(const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(AudioStream::eStreamTypes)

#endif // AUDIOSTREAM_H
