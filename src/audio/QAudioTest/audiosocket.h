#ifndef AUDIOSOCKET_H
#define AUDIOSOCKET_H

#include <QThread>
#include <QAbstractSocket>
#include <QUdpSocket>

#include "audiostream.h"
#include "encodedsample.h"

#define AUDIO_PORT  1337

class AudioSocket : public QThread
{
    Q_OBJECT

public:
    AudioSocket(QObject *parent = 0);
    virtual ~AudioSocket();

    inline void setInput(AudioStream *input) { _input = input; }
    inline const AudioStream* input() const { return _input; }

    inline void setOutput(AudioStream *output) { _output = output; }
    inline const AudioStream* output() const { return _output; }

public slots:
    void setHostAddr(const QHostAddress &addr);
    void setGain(int gain);

    void quit();
    void terminate();

protected:
    virtual void run();

private:
    bool _run;
    QUdpSocket *_socket;
    QHostAddress _hostAddr;
    AudioStream *_input;
    AudioStream *_output;
    EncodedSample *_encodedSample;

private slots:
    void _socket_readyRead();
};

#endif // AUDIOSOCKET_H
