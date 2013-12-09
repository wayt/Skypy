#include "SessionSocket.hpp"
#include <boost/bind.hpp>
#include <iostream>
#include "Utils.hpp"
#include "SocketMgr.hpp"

SessionSocket::SessionSocket(SocketMgr* mgr) : TcpSocket(mgr->io_service()),
    _sockMgr(mgr), _status(STATUS_UNAUTHED)
{}

void SessionSocket::init()
{
    _status = STATUS_UNAUTHED;

    uint8 buff[] = "WELCOME";
    send(buff, 8);

    _registerHeader();
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

    uint16_t size = (*((uint16 const*)&_header[0]));
    Utils::endian::big_to_native<uint16_t>(size);
    uint16_t opcode = (*((uint16 const*)&_header[2]));
    Utils::endian::big_to_native<uint16_t>(opcode);

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

    Packet pkt(code, _body, uint16_t(inputSize));
    handlePacketInput(pkt);

    _registerHeader();
}

void SessionSocket::send(Packet const& pkt)
{
    std::cerr << "SEND: " << uint32(pkt.getOpcode()) << std::endl;
    send(pkt.content(), pkt.size());
}

void SessionSocket::send(uint8 const* data, uint16 size)
{
    boost::asio::async_write(_socket,
            boost::asio::buffer(data, size),
            boost::bind(&SessionSocket::_handleWrite, this,
                boost::asio::placeholders::error));
}

void SessionSocket::_handleWrite(boost::system::error_code const& error)
{
    if (error)
    {
        std::cerr << "SessionSocket::_handleWrite: " << error.message() << std::endl;
        _sockMgr->handleWriteError(this, std::error_code(/*error.value()*/));
    }
}

void SessionSocket::handlePacketInput(Packet& pkt)
{
    Opcodes::OpcodeDefinition const* opcode = _sockMgr->getOpcodesMgr()->getOpcodeDefinition(pkt.getOpcode(), (_status == STATUS_UNAUTHED ? OPSTATUS_SYNC_UNAUTHED : OPSTATUS_SYNC_AUTHED));
    if (opcode)
    {

    }
    else

}