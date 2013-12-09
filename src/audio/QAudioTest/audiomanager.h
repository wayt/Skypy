#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <QThread>

#include "singleton.h"
#include "audiostream.h"
#include "encodedsample.h"

class AudioManager : public QThread, public Singleton<AudioManager>
{
    Q_OBJECT

public:
    AudioManager(QObject *parent = 0);
    virtual ~AudioManager();

    inline const QString& errorText() const { return _errText; }

    bool setInputDevice(int device = DEFAULT_DEVICE, AudioSample::eChannel channel = AudioSample::MONO, AudioSample::eFrequency frequency = AudioSample::FREQ_48000);
    bool setOutputDevice(int device = DEFAULT_DEVICE, AudioSample::eChannel channel = AudioSample::MONO, AudioSample::eFrequency frequency = AudioSample::FREQ_48000);

    AudioStream* input() { return _input; }
    const AudioStream* input() const { return _input; }
    AudioStream* output() { return _output; }
    const AudioStream* output() const { return _output; }

    inline void push(const EncodedSample &encodedSample) { _outputQueue.enqueue(encodedSample); }

    const QSynchronizedQueue<EncodedSample>& inputQueue() const { return _inputQueue; }
    QSynchronizedQueue<EncodedSample>& inputQueue() { return _inputQueue; }
    const QSynchronizedQueue<EncodedSample>& outputQueue() const { return _outputQueue; }
    QSynchronizedQueue<EncodedSample>& outputQueue() { return _outputQueue; }

public slots:
    bool start();
    void quit();
    void terminate();

protected:
    virtual void run();

private:
    bool _run;
    QString _errText;
    AudioStream *_input;
    AudioStream *_output;

    QSynchronizedQueue<EncodedSample> _inputQueue;
    QSynchronizedQueue<EncodedSample> _outputQueue;

signals:
    void error(const QString &errText);
};

#define sAudioManager AudioManager::instance()

#endif // AUDIOMANAGER_H
