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
    SocketMgr() : _service(), _serviceThreads(), _srvSock(NULL), _newSocks(),
    _newSocksMutex()
    {}

    bool startNetwork(unsigned short port, unsigned int threadCount = 1);

    boost::asio::io_service& io_service() { return _service; }
    virtual void registerNewSock(SessionSocket *);

    void shutdown();
    void wait() { _serviceThreads.join_all(); }

    void handleHeaderError(SessionSocket* sock, std::error_code const& error);
    void handleBodyError(SessionSocket* sock, std::error_code const& error);
    void handleInvalidHeaderSize(SessionSocket* sock, uint16_t size);
    void handleWriteError(SessionSocket* sock, std::error_code const& error);

    void addNewSock(SessionSocket* newSock);
    void removeNewSock(SessionSocket* newSock);
private:
    void clearOldSock();

    boost::asio::io_service _service;
    ThreadPool _serviceThreads;
    ServerSocket* _srvSock;
    std::map<SessionSocket*, uint32> _newSocks;
    Mutex _newSocksMutex;
};

#endif /* !SOCKETMGR_H_ */
