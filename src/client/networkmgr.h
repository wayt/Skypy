#ifndef NETWORKMGR_H
#define NETWORKMGR_H

#include <cstdlib>
#include <QString>
#include <QTcpSocket>

class NetworkMgr : public QObject
{
    Q_OBJECT

public:
    explicit NetworkMgr();

    static NetworkMgr* Instance();

    void tcpConnect(QString const& addr, quint16 port);

private slots:
    void handleConnected();
    void handleError(QAbstractSocket::SocketError e);

private:
    QTcpSocket _tcpSock;
};

#define sNetworkMgr NetworkMgr::Instance()

#endif // NETWORKMGR_H
