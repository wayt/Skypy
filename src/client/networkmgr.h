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

class sipRequest;
class sipRespond;
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
    void makeCall(const std::string &userName, const std::string &userAdress, const std::string &contactName, quint32 peerId);

    void closeTcpConnection();
    void setMainWindow(MainWindow* window) { _window = window; }

    void setCallHostAddr(const QHostAddress& addr, quint16 port = AUDIO_PORT) { _audioSock.setHostAddr(addr, port); }
    void setCallPeerAddr(const QHostAddress& addr, quint16 port = AUDIO_PORT) { _audioSock.setPeerAddr(addr, port); }
    void quitCall() { _audioSock.quit(); }
    void terminateCall() { _audioSock.terminate(); }
    void runCall() { _audioSock.start(); }
    void handleSipRequest(Packet &pkt);
    void handleSipRep(Packet &pkt);
    std::vector< std::pair< sipRequest*, sipRespond* > > getSipPool() { return (_sipPool); }
private slots:
    void _readInput();
    void _handleTcpConnected();
    void _handleTcpError(QAbstractSocket::SocketError e);
private:
    std::vector< std::pair< sipRequest*, sipRespond* > > _sipPool;
    QTcpSocket _tcpSock;
    MainWindow* _window;
    ConnectionState _connState;
    AudioSocket _audioSock;
};

#define sNetworkMgr NetworkMgr::instance()

#endif // NETWORKMGR_H
