#include "ServerSocket.hpp"

using boost::asio::ip::tcp;

ServerSocket::ServerSocket(SocketMgr* mgr, std::string const& addr, unsigned short port) :
    _sockMgr(mgr), _acceptor(mgr->NetService(), tcp::endpoint(addr, port))
{}

void ServerSocket::StartAccept()
{
    SessionSocket* newSock = new SessionSocket(_service);
    _acceptor.async_accept(newSock->socket(),
            boost::bind(&ServerSocket::HandleAccept, this, newSock, boost::asio::placeholders::error)
            );
}

void ServerSocket::HandleAccept(SessionSocket* newSock, boost::system::error_code const& error)
{
    if (!error)
        _sockMgr->RegisterNewSock(newSock);
    else
    {
        std::cerr << "ServerSocket::HandleAccept : " << error.message() << std::endl;
        delete newSock;
    }
    StartAccept();
}

