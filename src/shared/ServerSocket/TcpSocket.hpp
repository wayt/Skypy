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

    void init();
    virtual void onInit() {}
    void close();

protected:
    void _send(uint8 const* data, uint16 size);
    void _handleWrite(boost::system::error_code const& error);

    tcp::socket _socket;
};

#endif /* !TCPSOCKET_H_ */
