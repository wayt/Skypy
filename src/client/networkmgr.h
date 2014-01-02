#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <cstdlib>
#include <vector>
#include <utility>
#include <QString>
#include <QTcpSocket>
#include "packet.hpp"
#include "singleton.h"
#include "audiosocket.h"

class SipRequest;
class SipRespond;
class MainWindow;

class NetworkMgr : public QObject, public Singleton<NetworkMgr>
{
    Q_OBJECT

public:
    explicit NetworkMgr();

    enum ConnectionState
    {
        STATE_WELCOMING     = 0,
        STATE_WAITING_AUTH  = 1,
        STATE_AUTHED        = 2,
        STATE_DISCONNECTED  = 3
    };

    void tcpConnect(QString const& addr, quint16 port);
    void tcpSendPacket(Packet const& pkt);

    void closeTcpConnection();
    void setMainWindow(MainWindow* window) { _window = window; }

    //bool setCallHostAddr(const QHostAddress& addr, quint16 port = AUDIO_PORT) { return _audioSock.setHostAddr(addr, port); }
    bool addCallHostAddr(quint32 peerId, QHostAddress const& addr, quint16 port);
    void setCallPeerAddr(quint32 peerId, const QHostAddress& addr, quint16 port);
    void quitCall();
    AudioSocket* findAudioSocket(quint32 peerId);
    void handleSipRequest(Packet &pkt);
    void handleSipRep(Packet &pkt);
    void handleSipInfo(SipRequest const& request);
    void handleSipInfoResponse(SipRespond const& resp);

    void handleAudioNoInput(AudioSocket* sock);
    void forwardToOtherAudioSocket(QByteArray const& data, quint32 peerId);

    void debugInput();
private slots:
    void _readInput();
    void _handleTcpConnected();
    void _handleTcpError(QAbstractSocket::SocketError e);
private:
    QTcpSocket _tcpSock;
    MainWindow* _window;
    ConnectionState _connState;
    QMap<quint32, AudioSocket*> _audioSocks;
};

#define sNetworkMgr NetworkMgr::instance()

#endif // NETWORKMGR_H
