#ifndef SESSIONSOCKET_H_
# define SESSIONSOCKET_H_

#include <boost/asio.hpp>
#include <stdint.h>
#include <system_error>
#include "Opcodes.hpp"
#include "Packet.hpp"

using boost::asio::ip::tcp;
class SocketMgr;

class SessionSocket
{
public:
    SessionSocket(SocketMgr* mgr);

    tcp::socket& socket() { return _socket; }

    void init();

    void handlePacketInput(Packet& pkt);

    void _handleHeader(boost::system::error_code const& error);
    void _handleBody(uint16_t code, boost::system::error_code const& error, std::size_t inputSize);

    void send(uint8 const* data, uint16 size);
private:
    void _registerHeader();
    void _handleWrite(boost::system::error_code const& error);

    tcp::socket _socket;
    unsigned char _header[Packet::HeaderSize];
    unsigned char _body[Packet::MaxBodySize];
    SocketMgr* _sockMgr;
    uint32 _pingTime;
    uint32 _latency;
};

#endif /* !SESSIONSOCKET_H_ */
