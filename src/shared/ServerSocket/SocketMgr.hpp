#ifndef SOCKETMGR_H_
# define SOCKETMGR_H_

#include <boost/asio.hpp>
#include "ServerSocket.hpp"

class SocketMgr
{
public:
    SocketMgr() : _service(), _srvSock(NULL), _threads()
    {}

    bool StartNetwork(std::string const& addr, unsigned short port, unsigned int threadCount = 1);

private:
    boost::asio::io_service _service;
    ServerSocket* _srvSock;
    boost::thread_group _threads;
};

#endif /* !SOCKETMGR_H_ */
