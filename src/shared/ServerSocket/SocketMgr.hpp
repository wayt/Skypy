#ifndef SOCKETMGR_H_
# define SOCKETMGR_H_

#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "ServerSocket.hpp"

class SocketMgr
{
public:
    SocketMgr() : _service(), _srvSock(NULL), _threads()
    {}

    bool startNetwork(unsigned short port, unsigned int threadCount = 1);

    boost::asio::io_service& io_service() { return _service; }
    boost::asio::io_service const& io_service() const { return _service; }
    virtual void registerNewSock(SessionSocket *) {}

    void shutdown();
    void wait() { _threads.join_all(); }
private:
    boost::asio::io_service _service;
    ServerSocket* _srvSock;
    boost::thread_group _threads;
};

#endif /* !SOCKETMGR_H_ */
