#include "networkmgr.h"
#include "mainwindow.h"
#include <QtNetwork>
#include <iostream>

NetworkMgr::NetworkMgr(MainWindow* window) : _tcpSock(), _window(window), _welcoming(true)
{
    QTcpSocket::connect(&_tcpSock, SIGNAL(connected()), _window, SLOT(handleTcpConnected()));
    QTcpSocket::connect(&_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), _window, SLOT(handleTcpError(QAbstractSocket::SocketError)));
    QTcpSocket::connect(&_tcpSock, SIGNAL(readyRead()), this, SLOT(_readInput()));
}

void NetworkMgr::tcpConnect(QString const& addr, quint16 port)
{
    std::cout << "CONNETING TO: " << addr.toStdString() << ":" << port << std::endl;
    _tcpSock.connectToHost(addr, port, QTcpSocket::ReadWrite);
}

void NetworkMgr::tcpSendPacket(Packet const& pkt)
{
    _tcpSock.write(pkt.content(), pkt.size());
}

void NetworkMgr::_readInput()
{
    QByteArray data = _tcpSock.read(_welcoming ? 8 : 4);
    if (_welcoming)
    {
        QString welcome = "WELCOME";
        if (welcome.compare(data) == 0)
        {
            std::cout << "WELCOME OK" << std::endl;
            _window->handleRequireAuth();
        }
    }
    std::cout << "DATA: " << data[0] << data[1] << data[2] << data[3] << std::endl;
}
