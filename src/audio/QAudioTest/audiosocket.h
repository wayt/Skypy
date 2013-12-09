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

public slots:
    void setHostAddr(const QHostAddress &addr);

    void quit();
    void terminate();

protected:
    virtual void run();

private:
    bool _run;
    QUdpSocket *_socket;
    QHostAddress _hostAddr;

private slots:
    void _socket_readyRead();
};

#endif // AUDIOSOCKET_H
