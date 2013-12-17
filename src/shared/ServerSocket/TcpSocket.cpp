#include "TcpSocket.hpp"
#include <iostream>
#include <boost/bind.hpp>

TcpSocket::TcpSocket(boost::asio::io_service& service) :
    _socket(service)
{
}

void TcpSocket::init()
{
    boost::asio::ip::tcp::no_delay option(true);
    _socket.set_option(option);

    onInit();
}

void TcpSocket::close()
{
    boost::system::error_code ec;
    _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
    _socket.close();

    onClose();
}

void TcpSocket::_send(uint8 const* data, uint16 size)
{
    boost::asio::async_write(_socket,
            boost::asio::buffer(data, size),
            boost::bind(&TcpSocket::_handleWrite, this,
                boost::asio::placeholders::error));
}

void TcpSocket::_handleWrite(boost::system::error_code const& error)
{
    if (error)
        std::cerr << "Error: " << error.message() << std::endl;
}
