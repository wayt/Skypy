#include "SessionSocket.h"
#include <boost/bind.hpp>
#include <iostream>
#include "Utils.hpp"
#include "SocketMgr.h"
#include "Session.h"
#include "Skypy.h"

SessionSocket::SessionSocket(SocketMgr* mgr) : TcpSocket(mgr->io_service()),
    _sockMgr(mgr), _status(STATUS_UNAUTHED), _session(NULL)
{}

void SessionSocket::onInit()
{
    _status = STATUS_UNAUTHED;

    uint8 buff[] = "WELCOME";
    _send(buff, 8);

    std::cout << "SOCKET INIT" << std::endl;

    _registerHeader();
}

void SessionSocket::onClose()
{
    if (getStatus() == STATUS_UNAUTHED)
        _sockMgr->removeNewSock(this);
    else if (_session)
        _session->logout();
}

void SessionSocket::_registerHeader()
{
    boost::asio::async_read(_socket, boost::asio::buffer(_header, Packet::HeaderSize),
            boost::bind(&SessionSocket::_handleHeader, this,
                boost::asio::placeholders::error));
}

void SessionSocket::_handleHeader(boost::system::error_code const& error)
{
    if (error)
    {
        _sockMgr->handleHeaderError(this, std::error_code(/*error.value()*/));
        return;
    }

    std::cout << "RECV: " << int32(_header[0])
       << " - " << int32(_header[1])
       << " - " << int32(_header[2])
       << " - " << int32(_header[3]) << std::endl;

    uint16_t size = (*((uint16_t const*)&_header[0]));
    Utils::endian::big_to_native<uint16_t>(size);
    uint16_t opcode = (*((uint16_t const*)&_header[2]));
    Utils::endian::big_to_native<uint16_t>(opcode);

    std::cout << "HEADER INPUT " << size << " - " << opcode << std::endl;
    if (size > Packet::MaxBodySize)
    {
        _sockMgr->handleInvalidHeaderSize(this, size);
        return;
    }

    boost::asio::async_read(_socket, boost::asio::buffer(_body, size),
            boost::bind(&SessionSocket::_handleBody, this, opcode,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void SessionSocket::_handleBody(uint16_t code, boost::system::error_code const& error, std::size_t inputSize)
{
    if (error)
    {
        _sockMgr->handleBodyError(this, std::error_code(/*error.value()*/));
        return;
    }
    std::cout << "BODY READED" << std::endl;

    Packet pkt(code, _body, uint16_t(inputSize));
    handlePacketInput(pkt);

    _registerHeader();
}

void SessionSocket::send(Packet const& pkt)
{
    std::cerr << "SEND: " << uint32(pkt.getOpcode()) << std::endl;
    _send(pkt.content(), pkt.size());
}

void SessionSocket::handlePacketInput(Packet& pkt)
{
  std::cout << "RECEIV PACKET: " << pkt.getOpcode() << std::endl;
  pkt.dumpHex();
    if (_status == STATUS_UNAUTHED) // Special cases
    {
        switch (pkt.getOpcode())
        {
            case CMSG_AUTH:
                _handleAuthRequest(pkt);
                break;
            default:
                break;
        }
        return;
    }

    if (_session)
        _session->handlePacketInput(pkt);
}

void SessionSocket::_handleAuthRequest(Packet& pkt)
{
    std::string email;
    std::string password;
    pkt >> email >> password;
    std::cout << "HANDLE AUTH: " << email << " - " << password << std::endl;

    _sockMgr->removeNewSock(this);
    _status = STATUS_AUTHED;

    static uint32 titi = 1;
    _session = new Session(titi++, this);

    sSkypy->addSession(_session);

    Packet data(SMSG_AUTH_RESULT);
    data << uint8(AUTHRESULT_OK);
    send(data);
}
