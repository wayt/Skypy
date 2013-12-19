#include <iostream>
#include "ServerSocket.h"
#include "SocketMgr.h"

using boost::asio::ip::tcp;

ServerSocket::ServerSocket(SocketMgr* mgr, unsigned short port) :
    _sockMgr(mgr), _acceptor(mgr->io_service(), tcp::endpoint(tcp::v4(), port))
{
    startAccept();
}

void ServerSocket::startAccept()
{
    SessionSocket* newSock = new SessionSocket(_sockMgr);
    _acceptor.async_accept(newSock->socket(),
            boost::bind(&ServerSocket::handleAccept, this, newSock, boost::asio::placeholders::error)
            );
}

void ServerSocket::handleAccept(SessionSocket* newSock, boost::system::error_code const& error)
{
    if (!error)
        _sockMgr->registerNewSock(newSock);
    else
    {
        ON_NETWORK_DEBUG(
                std::cerr << "Network: ServerSocket::HandleAccept : " << error.message() << std::endl;
        );
        delete newSock;
    }
    startAccept();
}

void ServerSocket::shutdown()
{
    _acceptor.cancel();
    _acceptor.close();
}

