#ifndef SESSIONSOCKET_H_
# define SESSIONSOCKET_H_

#include <boost/asio.hpp>
#include <stdint.h>
#include <system_error>
#include "Opcodes.hpp"
#include "Packet.hpp"

using boost::asio::ip::tcp;

class SessionSocket
{
public:
    SessionSocket(boost::asio::io_service& service) :
        _socket(service)
    {}

    tcp::socket& socket() { return _socket; }

    void init();

    virtual void handleHeaderError(std::error_code const& error) {}
    virtual void handleBodyError(std::error_code const& error) {}
    virtual void handleInvalidHeaderSize(uint16_t size) {}

    virtual void handlePacketInput(Packet& pkt) {}

    void _handleHeader(boost::system::error_code const& error);
    void _handleBody(uint16_t code, boost::system::error_code const& error, std::size_t inputSize);
private:
    void _registerHeader();

    tcp::socket _socket;
    unsigned char _header[Packet::HeaderSize];
    unsigned char _body[Packet::MaxBodySize];
};

#endif /* !SESSIONSOCKET_H_ */
