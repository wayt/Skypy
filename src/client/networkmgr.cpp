#include "networkmgr.h"
#include "opcodemgr.h"
#include "sipPacket.hpp"
#include "mainwindow.h"
#include <QtNetwork>
#include <iostream>
#include "opcodemgr.h"
#include <QtEndian>

NetworkMgr::NetworkMgr() : _tcpSock(), _window(NULL), _connState(STATE_DISCONNECTED),
    _audioSock(this)
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
    char buff[8];
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
           quint16 buffSize = (size == 0 ? 1 : size);
           char* data = new char[buffSize];
           _tcpSock.read(data, size);
           Packet pkt(code, data, size);
           delete data;
           if (_window->handleAuthResult(pkt))
               _connState = STATE_AUTHED;
           break;
       }
       case STATE_AUTHED:
       {
           quint16 size = qFromBigEndian<quint16>(*((quint16 const*)&buff[0]));
           quint16 code = qFromBigEndian<quint16>(*((quint16 const*)&buff[2]));

           std::cout << "RECEIV SIZE: " << size << " - CODE : " << code << std::endl;
           quint16 buffSize = (size == 0 ? 1 : size);
           char* data = new char[buffSize];
           _tcpSock.read(data, size);
           Packet pkt(code, data, size);
           delete data;
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

void NetworkMgr::makeCall(const std::string &userName, const std::string &contactName, const std::string &contactAdress, quint32 peerId)
{
    Packet pkt(RMSG_SIP);
    const std::string cmd("INVITE");
    sipRequest *Rqst = new sipRequest(cmd, userName, contactName, contactAdress, peerId);
    _sipPool.push_back(std::make_pair(Rqst, (sipRespond *) NULL));
    this->tcpSendPacket(Rqst->getPacket());
}

void  NetworkMgr::handleSipRep(Packet &pkt)
{
    int code;
    std::string cmd;
    quint32 peerId;
    std::string user;
    std::string adress;
    std::string name;
    std::string myaddr;
    int port;
    bool accept =true;

    std::cout << "SIP RESPOND RECEIVED" << std::endl;
    pkt >> code >> cmd >> user >> name >> adress >> myaddr >> port >> peerId;
    for (std::vector< std::pair<sipRequest*, sipRespond*> >::iterator it = _sipPool.begin() ; it != _sipPool.end(); ++it)
    {
        if (((*it).first)->getCmd() == cmd && ((*it).first)->getUserName() == user && ((*it).first)->getContactName() == name)
        {
            (*it).second = new sipRespond(code, cmd, user, name, adress, myaddr, port, peerId);
            _window->handlesipResponse(it->first, it->second);
            return;
        }
    }
}

void NetworkMgr::handleSipRequest(Packet &pkt)
{
    std::string cmd;
    quint32 peerId;
    std::string userName;
    std::string contactAdress;
    std::string contactName;
    bool accept =true;

    std::cout << "SIP REQUESTPACKET RECEIVED" << std::endl;
    pkt >> cmd;
    pkt >> userName >> contactName >> contactAdress >> peerId;
    if (cmd == "INVITE")
    {
        std::cout << userName << " wanna start a vocal conversation with you" << std::endl;
        _window->handleCallRequest(userName, contactName, contactAdress, peerId);
       /*
        if (accept == true)
        {
            sipRespond *Rep = new sipRespond(200, cmd, userName, contactName, contactAdress, "my adress", 4242, peerId);
            sNetworkMgr->tcpSendPacket(Rep->getPacket());
        }
        else
        {
            sipRespond *Rep = new sipRespond(603, cmd, userName, contactName, contactAdress, NULL, 0, peerId);
            sNetworkMgr->tcpSendPacket(Rep->getPacket());
        }
        */
    }
}
