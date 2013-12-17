#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <cstdlib>
#include <QString>
#include <QTcpSocket>
#include "packet.hpp"

class MainWindow;

class NetworkMgr : public QObject
{
    Q_OBJECT

public:
    explicit NetworkMgr(MainWindow* window);

    enum ConnectionState
    {
        STATE_WELCOMING     = 0,
        STATE_WAITING_AUTH  = 1,
        STATE_AUTHED        = 2,
        STATE_DISCONNECTED  = 3
    };

    void tcpConnect(QString const& addr, quint16 port);
    void tcpSendPacket(Packet const& pkt);
    void makeCall(const std::string &userName, const std::string &userAdress, const std::string &contactName);

    void closeTcpConnection();

private slots:
    void _readInput();
    void _handleTcpConnected();
    void _handleTcpError(QAbstractSocket::SocketError e);

private:

    QTcpSocket _tcpSock;
    MainWindow* _window;
    ConnectionState _connState;
};

#endif // NETWORKMGR_H
