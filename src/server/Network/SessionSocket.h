#ifndef SESSIONSOCKET_H_
# define SESSIONSOCKET_H_

#include <stdint.h>
#include <system_error>
#include "Opcodes.h"
#include "Packet.hpp"
#include "TcpSocket.hpp"

class SocketMgr;
class Session;

enum SocketStatus
{
    STATUS_UNAUTHED     = 0,
    STATUS_AUTHED       = 1
};

enum AuthResults
{
    AUTHRESULT_OK           = 0,
    AUTHRESULT_BAD_LOG      = 1,
    AUTHRESULT_INTERAL_ERR  = 2,
};

class SessionSocket : public TcpSocket
{
public:
    SessionSocket(SocketMgr* mgr);

    tcp::socket& socket() { return _socket; }

    void onInit();

    void handlePacketInput(Packet& pkt);

    void _handleHeader(boost::system::error_code const& error);
    void _handleBody(uint16_t code, boost::system::error_code const& error, std::size_t inputSize);

    void send(Packet const& pkt);

    SocketStatus getStatus() const { return _status; }
private:
    void _registerHeader();
    void _handleWrite(boost::system::error_code const& error);

    void _handleAuthRequest(Packet& pkt);

    unsigned char _header[Packet::HeaderSize];
    unsigned char _body[Packet::MaxBodySize];
    SocketMgr* _sockMgr;
    SocketStatus _status;
    Session* _session;
};

#endif /* !SESSIONSOCKET_H_ */
