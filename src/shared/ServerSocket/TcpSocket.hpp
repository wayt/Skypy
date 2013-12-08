#ifndef TCPSOCKET_H_
# define TCPSOCKET_H_

#include <boost/asio.hpp>
#include "Utils.hpp"

using boost::asio::ip::tcp;

class TcpSocket
{
public:
    TcpSocket(boost::asio::io_service& service);

    tcp::socket& socket() { return _socket; }

    void close();

protected:
    void send(uint8 const* data, uint16 size);

    tcp::socket _socket;
};

#endif /* !TCPSOCKET_H_ */
