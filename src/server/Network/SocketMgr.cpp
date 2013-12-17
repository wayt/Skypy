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
        _thread = new Thread(&_service);
    } catch (std::exception const& e) {
        std::cerr << "Fail to start network: " << e.what() << std::endl;
        return false;
    }
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

    if (_thread)
        _thread->join();
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
            delete sock;
        }
        ++itr;
    }
}

void SocketMgr::handleHeaderError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleHeaderError" << std::endl;
    if (sock->getStatus() == STATUS_UNAUTHED)
    {
        removeNewSock(sock);
        sock->close();
        delete sock;
    }
}

void SocketMgr::handleBodyError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleBodyError" << std::endl;
    if (sock->getStatus() == STATUS_UNAUTHED)
    {
        removeNewSock(sock);
        sock->close();
        delete sock;
    }
}

void SocketMgr::handleInvalidHeaderSize(SessionSocket* sock, uint16_t size)
{
    std::cout << "handleInvalidHeaderSize" << std::endl;
    if (sock->getStatus() == STATUS_UNAUTHED)
    {
        removeNewSock(sock);
        sock->close();
        delete sock;
    }
}

void SocketMgr::handleWriteError(SessionSocket* sock, std::error_code const& error)
{
    std::cout << "handleWriteError" << std::endl;
    if (sock->getStatus() == STATUS_UNAUTHED)
    {
        removeNewSock(sock);
        sock->close();
        delete sock;
    }
}
