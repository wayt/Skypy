#include "networkmgr.h"
#include "opcodemgr.h"
#include "sipPacket.hpp"
#include "mainwindow.h"
#include <QtNetwork>
#include <iostream>
#include "opcodemgr.h"
#include <QtEndian>
#include "clientmgr.h"

NetworkMgr::NetworkMgr() : _tcpSock(), _window(NULL), _connState(STATE_DISCONNECTED),
    _audioSocks()
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
    while (_tcpSock.bytesAvailable() > 0)
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
                    _window->handleRequireAuth(_tcpSock.localAddress().toString());
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
                char data[Packet::MaxBodySize];
                if (size > 0 && size < Packet::MaxBodySize)
                    _tcpSock.read(data, size);
                Packet pkt(code, data, size);
                if (_window->handleAuthResult(pkt))
                {
                    sClientMgr->setPrivateIp(_tcpSock.localAddress().toString());
                    _connState = STATE_AUTHED;
                }
                break;
            }
            case STATE_AUTHED:
            {
                quint16 size = qFromBigEndian<quint16>(*((quint16 const*)&buff[0]));
                quint16 code = qFromBigEndian<quint16>(*((quint16 const*)&buff[2]));

                std::cout << "RECEIV SIZE: " << size << " - CODE : " << code << std::endl;
                char data[Packet::MaxBodySize];
                if (size > 0 && size << Packet::MaxBodySize)
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
}

void NetworkMgr::debugInput()
{
    qint64 size = _tcpSock.bytesAvailable();
    std::cout << "DEBUG INPUT SIZE: " << size << std::endl;
    if (size > 0)
    {
        char buff[Packet::MaxBodySize];
        _tcpSock.read(buff, size);
        for (qint64 i = 0; i < size; ++i)
        {
            std::cout << " - " << std::hex << qint32(buff[i]);
            if (i % 4 == 0)
                std::cout << std::endl;
        }
    }
}

void  NetworkMgr::handleSipRep(Packet &pkt)
{
    quint32 code;
    QString cmd;
    QString senderEmail;
    quint32 senderId;
    QString senderIp;
    quint32 senderPort;
    QString destEmail;
    quint32 destId;
    QString destIp;
    quint32 destPort;
    quint32 chatId;

    std::cout << "SIP RESPOND RECEIVED" << std::endl;
    pkt >> code >> cmd;
    pkt >> senderEmail >> senderId >> senderIp >> senderPort;
    pkt >> destEmail >> destId >> destIp >> destPort;
    pkt >> chatId;
    SipRespond resp(code, cmd, senderEmail, senderId, senderIp, senderPort, destEmail, destId, destIp, destPort, chatId);
    if (resp.getCmd() != "INFO")
        _window->handlesipResponse(resp);
    else
        handleSipInfoResponse(resp);
}

void NetworkMgr::handleSipRequest(Packet &pkt)
{
    QString cmd;
    QString senderEmail;
    quint32 senderId;
    QString senderIp;
    quint32 senderPort;
    QString destEmail;
    quint32 destId;
    QString destIp;
    quint32 destPort;

    std::cout << "SIP REQUESTPACKET RECEIVED" << std::endl;
    pkt >> cmd;
    pkt >> senderEmail >> senderId >> senderIp >> senderPort;
    pkt >> destEmail >> destId >> destIp >> destPort;
    SipRequest request(cmd, senderEmail, senderId, senderIp, senderPort, destEmail, destId, destIp, destPort);

    if (cmd == "INVITE")
    {
        std::cout << senderEmail.toStdString() << " wanna start a vocal conversation with you" << std::endl;
        _window->handleCallRequest(request);
    }
    else if (cmd == "BYE")
    {
        _window->handleByeRequest(request);
    }
    else if (cmd == "INFO")
    {
        handleSipInfo(request);
    }
}

void NetworkMgr::handleAudioNoInput(AudioSocket* sock)
{
    if (!sClientMgr->hasActiveCall())
        return;

    QHostAddress hostAddr, peerAddr;
    quint16 hostPort, peerPort;
    sock->getHostInfo(hostAddr, hostPort);
    sock->getPeerInfo(peerAddr, peerPort);

    CallPeer const* peer = sClientMgr->getCallPeer(peerAddr.toString(), peerPort);

    quint32 selfPort = hostPort + 1;
    SipRequest Rqst("INFO", sClientMgr->getEmail(), sClientMgr->getAccountId(), hostAddr.toString(), selfPort, peer->email, peer->id, peerAddr.toString(), peerPort);
    sNetworkMgr->tcpSendPacket(Rqst.getPacket());
    std::cout << "NEW HOST PORT: " << selfPort << std::endl;
}

void NetworkMgr::handleSipInfo(SipRequest const& request)
{
    if (!sClientMgr->hasActiveCallWith(request.getSenderId()))
        return;

    std::cout << "NEW PEER PORT: " << request.getSenderPort() << std::endl;
    sNetworkMgr->setCallPeerAddr(QHostAddress(request.getDestIp()), request.getDestPort(), QHostAddress(request.getSenderIp()), request.getSenderPort());
    if (CallPeer* peer = sClientMgr->getCallPeer(request.getSenderId()))
        peer->port = request.getSenderPort();

    SipRespond resp(200, request);
    tcpSendPacket(resp.getPacket());
}

void NetworkMgr::handleSipInfoResponse(SipRespond const& resp)
{
    if (resp.getCode() == 180)
    {
        if (AudioSocket* sock = findAudioSocket(QHostAddress(resp.getDestIp()), resp.getDestPort()))
            sock->setHostAddr(QHostAddress(resp.getSenderIp()), resp.getSenderPort());
    }


}

bool NetworkMgr::addCallHostAddr(QHostAddress const& addr, quint16 port)
{
    AudioSocket* sock = new AudioSocket(this);
    if (!sock->setHostAddr(addr, port))
    {
        delete sock;
        return false;
    }
    _audioSocks.push_back(sock);
    return true;
}

void NetworkMgr::quitCall()
{
    for (QList<AudioSocket*>::ConstIterator itr = _audioSocks.begin();
         itr != _audioSocks.end();)
    {
        AudioSocket* sock = *itr;
        ++itr;
        sock->quit();
        delete sock;
    }
    _audioSocks.clear();
}

void NetworkMgr::setCallPeerAddr(QHostAddress const& hostAddr, quint16 hostPort, const QHostAddress& addr, quint16 port)
{
    if (AudioSocket* sock = findAudioSocket(hostAddr, hostPort))
    {
        sock->setHostAddr(addr, port);
        if (!sock->isRunning())
            sock->start();
    }
}

AudioSocket* NetworkMgr::findAudioSocket(QHostAddress const& host, quint16 port)
{
    for (QList<AudioSocket*>::ConstIterator itr = _audioSocks.begin();
         itr != _audioSocks.end(); ++itr)
    {
        AudioSocket* sock = *itr;
        QHostAddress currHostAddr;
        quint16 currHostPort;
        sock->getHostInfo(currHostAddr, currHostPort);

        if (host.toString() == currHostAddr.toString() && currHostPort == port)
            return sock;
    }
    return NULL;
}
