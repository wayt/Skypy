#include "networkmgr.h"
#include "opcodemgr.h"
#include "mainwindow.h"
#include <QtNetwork>
#include <iostream>
#include "opcodemgr.h"
#include <QtEndian>

NetworkMgr::NetworkMgr() : _tcpSock(), _window(NULL), _connState(STATE_DISCONNECTED)
{
    QTcpSocket::connect(&_tcpSock, SIGNAL(connected()), this, SLOT(_handleTcpConnected()));
    QTcpSocket::connect(&_tcpSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(_handleTcpError(QAbstractSocket::SocketError)));
    QTcpSocket::connect(&_tcpSock, SIGNAL(readyRead()), this, SLOT(_readInput()));
}

void NetworkMgr::tcpConnect(QString const& addr, quint16 port)
{
    if (_connState != STATE_DISCONNECTED)
        return;
    std::cout << "CONNETING TO: " << addr.toStdString() << ":" << port << std::endl;
    _tcpSock.connectToHost(addr, port, QTcpSocket::ReadWrite);
}

void NetworkMgr::tcpSendPacket(Packet const& pkt)
{
    _tcpSock.write(pkt.content(), pkt.size());
}

void NetworkMgr::_handleTcpConnected()
{
    if (_connState != STATE_DISCONNECTED)
        return;
    _connState = STATE_WELCOMING;
    std::cout << "SOCKET CONNECTED" << std::endl;
}

void NetworkMgr::_handleTcpError(QAbstractSocket::SocketError e)
{
    std::cerr << "TCP Error " << qint32(e) << std::endl;
    if (_connState == STATE_AUTHED)
        _window->handleServerConnectionLost(e, _tcpSock.errorString());
    closeTcpConnection();
}

void NetworkMgr::closeTcpConnection()
{
    _connState = STATE_DISCONNECTED;
    _tcpSock.close();
}

void NetworkMgr::_readInput()
{
    std::cout << "CALL READ INPUT" << std::endl;
    bool welcoming = _connState == STATE_WELCOMING;
    char buff[welcoming ? 8 : 4];
    std::cout << "AVAILABLE: " << _tcpSock.bytesAvailable() << " _connState: " << quint32(_connState) << std::endl;
   _tcpSock.read(buff, welcoming ? 8 : 4);

   switch (_connState)
   {
       case STATE_DISCONNECTED:
           break;
       case STATE_WELCOMING:
       {
           QString welcome = "WELCOME";
            if (welcome.compare(buff) == 0)
            {
                std::cout << "WELCOME OK" << std::endl;
                _window->handleRequireAuth();
                _connState = STATE_WAITING_AUTH;
            }
            else
                closeTcpConnection();
           break;
       }
       case STATE_WAITING_AUTH:
       {
           quint16 size = qFromBigEndian<quint16>(*((quint16 const*)&buff[0]));
           quint16 code = qFromBigEndian<quint16>(*((quint16 const*)&buff[2]));
           if (size != 1 || code != SMSG_AUTH_RESULT)
           {
               closeTcpConnection();
               break;
           }

           char data[size];
           _tcpSock.read(data, size);
           Packet pkt(code, data, size);
           if (_window->handleAuthResult(pkt))
               _connState = STATE_AUTHED;
           break;
       }
       case STATE_AUTHED:
       {
           quint16 size = qFromBigEndian<quint16>(*((quint16 const*)&buff[0]));
           quint16 code = qFromBigEndian<quint16>(*((quint16 const*)&buff[2]));

           std::cout << "RECEIV SIZE: " << size << " - CODE : " << code << std::endl;
           char data[size];
           _tcpSock.read(data, size);
           Packet pkt(code, data, size);

           pkt.dumpHex();

           OpcodeMgr::OpcodeDefinition const* opcodedef = OpcodeMgr::getOpcodeDefinition(pkt.getOpcode());
           if (!opcodedef)
           {
               std::cerr << "Error: receiv unknow opcode: " << pkt.getOpcode() << std::endl;
               return;
           }

           if (opcodedef->func)
               (_window->*opcodedef->func)(pkt);
           break;
       }
    }
}

void NetworkMgr::makeCall(const std::string &userName, const std::string &userAdress, const std::string &contactName)
{
  Packet pkt(CMSG_SIP);
  const std::string cmd("RINVITE");
  QHostAddress addr(QHostAddress::LocalHost);

  pkt << cmd;
  pkt << userName;
  pkt << ((addr.toString()).toStdString());
  pkt << contactName;
  this->tcpSendPacket(pkt);
}
