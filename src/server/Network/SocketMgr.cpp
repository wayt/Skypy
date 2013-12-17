#include <functional>
#include "SocketMgr.h"
#include "ServerSocket.h"
#include "SessionSocket.h"
#include <iostream>

bool SocketMgr::startNetwork(unsigned short port, unsigned int threadCount)
{
    if (threadCount == 0)
    {
        std::cerr << "Error: invalide network thread count: " << threadCount << std::endl;
        return false;
    }

    try {
        _srvSock = new ServerSocket(this, port);
        for (uint32 i = 0; i < threadCount; ++i)
            _serviceThreads.create_thread(&_service);
    } catch (std::exception const& e) {
        std::cerr << "Fail to start network: " << e.what() << std::endl;
        return false;
    }
    std::cout << "Network started on port " << port << " with " << threadCount << " threads" << std::endl;
    return true;
}

void SocketMgr::registerNewSock(SessionSocket* newSock)
{
    std::cout << "registerNewSock" << std::endl;
    clearOldSock();
    addNewSock(newSock);
    newSock->init();
}

void SocketMgr::shutdown()
{
    if (_srvSock)
        _srvSock->shutdown();

    _service.stop();
}

void SocketMgr::addNewSock(SessionSocket* newSock)
{
    Mutex::ScopLock lock(_newSocksMutex);
    _newSocks[newSock] = Utils::getMSTime() + 10 * IN_MILLISECONDS;
}

void SocketMgr::removeNewSock(SessionSocket* newSock)
{
    Mutex::ScopLock lock(_newSocksMutex);
    _newSocks.erase(newSock);
}

void SocketMgr::clearOldSock()
{
    Mutex::ScopLock lock(_newSocksMutex);

    uint32 msTime = Utils::getMSTime();
    for (std::map<SessionSocket*, uint32>::iterator itr = _newSocks.begin(); itr != _newSocks.end();)
    {
        if (itr->second >= msTime)
        {
            SessionSocket* sock = itr->first;
            _newSocks.erase(itr++);
            sock->close();
        }
        ++itr;
    }
}

void SocketMgr::handleHeaderError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleHeaderError" << std::endl;
    sock->close();
}

void SocketMgr::handleBodyError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleBodyError" << std::endl;
    sock->close();
}

void SocketMgr::handleInvalidHeaderSize(SessionSocket* sock, uint16_t size)
{
    std::cout << "handleInvalidHeaderSize" << std::endl;
    sock->close();
}

void SocketMgr::handleWriteError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleWriteError" << std::endl;
    sock->close();
}
