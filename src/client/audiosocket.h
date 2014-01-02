#ifndef AUDIOSOCKET_H
#define AUDIOSOCKET_H

#include <QThread>
#include <QAbstractSocket>
#include <QUdpSocket>

#include "audiostream.h"
#include "encodedsample.h"

#define AUDIO_PORT  4242

class AudioSocket : public QThread
{
    Q_OBJECT

public:
    AudioSocket(QObject *parent = 0);
    virtual ~AudioSocket();

public slots:
    bool setHostAddr(const QHostAddress &addr, quint16 port = AUDIO_PORT);
    void setPeerAddr(const QHostAddress& addr, quint16 port = AUDIO_PORT);

    void quit();
    void terminate();
    virtual void run();

    void getHostInfo(QHostAddress& addr, quint16& port) const { addr = _hostAddr; port = _hostPort; }
    void getPeerInfo(QHostAddress& addr, quint16& port) const { addr = _peerAddr; port = _peerPort; }
private:
    bool _run;
    QUdpSocket *_socket;
    QHostAddress _hostAddr;
    quint16 _hostPort;
    QHostAddress _peerAddr;
    quint16 _peerPort;

    bool _inputReaded;

private slots:
    void _socket_readyRead();
};

#endif // AUDIOSOCKET_H
