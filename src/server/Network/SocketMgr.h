#ifndef SOCKETMGR_H_
# define SOCKETMGR_H_

#include <boost/asio.hpp>
#include "ThreadPool.hpp"
#include "ServerSocket.h"
#include "Mutex.hpp"

class SessionSocket;

class SocketMgr
{
public:
    SocketMgr() : _service(), _srvSock(NULL), _thread(NULL), _newSocks(),
    _newSocksMutex()
    {}

    bool startNetwork(unsigned short port, unsigned int threadCount = 1);

    boost::asio::io_service& io_service() { return _service; }
    boost::asio::io_service const& io_service() const { return _service; }
    virtual void registerNewSock(SessionSocket *);

    void shutdown();
    void wait() { if (_thread) _thread->join(); }

    void handleHeaderError(SessionSocket* sock, std::error_code const& error);
    void handleBodyError(SessionSocket* sock, std::error_code const& error);
    void handleInvalidHeaderSize(SessionSocket* sock, uint16_t size);
    void handleWriteError(SessionSocket* sock, std::error_code const& error);

    void addNewSock(SessionSocket* newSock);
    void removeNewSock(SessionSocket* newSock);
private:
    void clearOldSock();

    boost::asio::io_service _service;
    ServerSocket* _srvSock;
    Thread* _thread;
    std::map<SessionSocket*, uint32> _newSocks;
    Mutex _newSocksMutex;
};

#endif /* !SOCKETMGR_H_ */
