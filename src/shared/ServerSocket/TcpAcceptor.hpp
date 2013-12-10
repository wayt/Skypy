#ifndef TCPACCEPTOR_H_
# define TCPACCEPTOR_H_

#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class TcpAcceptor : public tcp::acceptor
{
public:
    TcpAcceptor(boost::asio::io_service& service, tcp::endpoint endpoint) :
        tcp::acceptor(service, endpoint)
    {}
};

#endif /* !TCPACCEPTOR_H_ */
