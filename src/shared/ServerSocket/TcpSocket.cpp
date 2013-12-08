#include "TcpSocket.hpp"

TcpSocket::TcpSocket(boost::asio::io_service& service) :
    _socket(service)
{
    boost::asio::ip::tcp::no_delay option(true);
    _socket.set_option(option);
}

void SessionSocket::close()
{
    boost::system::error_code ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _socket.close();
}

void SessionSocket::send(uint8 const* data, uint16 size)
{
    boost::asio::async_write(_socket,
            boost::asio::buffer(data, size),
            boost::bind(&SessionSocket::_handleWrite, this,
                boost::asio::placeholders::error));
}
