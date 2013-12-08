#ifndef TCPACCEPTOR_H_
# define TCPACCEPTOR_H_

#include <boost/asio.hpp>

class TcpAcceptor : public boost::asio::ip::tcp::acceptor
{
public:
};

#endif /* !TCPACCEPTOR_H_ */
