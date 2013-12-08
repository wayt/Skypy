#ifndef SOCKETMGR_H_
# define SOCKETMGR_H_

#include <boost/asio.hpp>
#include "ThreadPool.hpp"
#include "ServerSocket.hpp"

class SocketMgr
{
public:
    SocketMgr() : _service(), _srvSock(NULL), _thread(NULL), _newSock()
    {}

    bool startNetwork(unsigned short port, unsigned int threadCount = 1);

    boost::asio::io_service& io_service() { return _service; }
    boost::asio::io_service const& io_service() const { return _service; }
    virtual void registerNewSock(SessionSocket *) {}

    void shutdown();
    void wait() { if (_thread) _thread->join(); }

    virtual void handleHeaderError(SessionSocket* sock, std::error_code const& error) {}
    virtual void handleBodyError(SessionSocket* sock, std::error_code const& error) {}
    virtual void handleInvalidHeaderSize(SessionSocket* sock, uint16_t size) {}
    virtual void handleWriteError(SessionSocket* sock, std::error_code const& error) {}


private:
    boost::asio::io_service _service;
    ServerSocket* _srvSock;
    Thread* _thread;
    std::list<SessionSocket*> _newSock;
};

#endif /* !SOCKETMGR_H_ */
