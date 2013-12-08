#ifndef SESSIONSOCKET_H_
# define SESSIONSOCKET_H_

#include <boost/asio.hpp>
#include <stdint.h>
#include <system_error>
#include "Opcodes.hpp"
#include "Packet.hpp"

using boost::asio::ip::tcp;
class SocketMgr;

enum SocketStatus
{
    STATUS_NONE         = 0,
    STATUS_UNAUTHED     = 1,
    STATUS_AUTHED       = 2
};

class SessionSocket
{
public:
    SessionSocket(SocketMgr* mgr);

    tcp::socket& socket() { return _socket; }

    void init();
    void close();

    void handlePacketInput(Packet& pkt);

    void _handleHeader(boost::system::error_code const& error);
    void _handleBody(uint16_t code, boost::system::error_code const& error, std::size_t inputSize);

    void send(Packet const& pkt);

    SocketStatus getStatus() const { return _status; }
private:
    void send(uint8 const* data, uint16 size);
    void _registerHeader();
    void _handleWrite(boost::system::error_code const& error);

    tcp::socket _socket;
    unsigned char _header[Packet::HeaderSize];
    unsigned char _body[Packet::MaxBodySize];
    SocketMgr* _sockMgr;
    uint32 _pingTime;
    uint32 _latency;
    SocketStatus _status;
};

#endif /* !SESSIONSOCKET_H_ */
