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

    void tcpConnect(QString const& addr, quint16 port);
    void tcpSendPacket(Packet const& pkt);

private slots:
    void _readInput();

private:
    QTcpSocket _tcpSock;
    MainWindow* _window;
    bool _welcoming;
};

#endif // NETWORKMGR_H
