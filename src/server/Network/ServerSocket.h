#ifndef SERVERSOCKET_H_
# define SERVERSOCKET_H_

#include "SessionSocket.h"
#include "TcpAcceptor.hpp"

class SocketMgr;

class ServerSocket
{
public:
    ServerSocket(SocketMgr* mgr, unsigned short port);

    void shutdown();
private:
    void startAccept();
    void handleAccept(SessionSocket* newSock, boost::system::error_code const& error);

    SocketMgr* _sockMgr;
    TcpAcceptor _acceptor;
};

#endif /* !SERVERSOCKET_H_ */
