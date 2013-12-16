#include "networkmgr.h"
#include <QtNetwork>
#include <iostream>

NetworkMgr::NetworkMgr()
{
    QTcpSocket::connect(&_tcpSock, SIGNAL(connected()), this, SLOT(handleConnected()));
    QTcpSocket::connect(&_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(handleError(QAbstractSocket::SocketError)));
    // signal émis lors de la connexion au serveur
    //QTcpSocket::connect(&_tcpSock, SIGNAL(readyRead()), this, SLOT(lecture()));
    // signal émis lorsque des données sont prêtes à être lues
}


NetworkMgr* NetworkMgr::Instance()
{
    static NetworkMgr* instance = NULL;
    if (!instance)
        instance = new NetworkMgr();
    return instance;
}


void NetworkMgr::tcpConnect(QString const& addr, quint16 port)
{
    std::cout << "CONNETING TO: " << addr.toStdString() << ":" << port << std::endl;
    _tcpSock.connectToHost(addr, port, QTcpSocket::ReadWrite);
}

void NetworkMgr::handleConnected()
{
    std::cout << "HANDLE CONNECTED" << std::endl;
}

void NetworkMgr::handleError(QAbstractSocket::SocketError e)
{
    std::cout << "HANDLE ERROR: " << e << std::endl;
}
