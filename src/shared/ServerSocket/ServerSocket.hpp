#ifndef SERVERSOCKET_H_
# define SERVERSOCKET_H_

#include <boost/asio.hpp>
#include "SessionSocket.hpp"

using boost::asio::ip::tcp;
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
    tcp::acceptor _acceptor;
};

#endif /* !SERVERSOCKET_H_ */
