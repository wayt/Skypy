#ifndef SERVERSOCKET_H_
# define SERVERSOCKET_H_

#include <boost/asio.hpp>

using boost::asio::ip::tcp;
class SocketMgr;

class ServerSocket
{
public:
    ServerSocket(SocketMgr* mgr, std::string const& addr, unsigned short port);

protected:
    NetService& _service;

private:
    void StartAccept();
    void HandleAccept();

    SocketMgr* _sockMgr;
    tcp::acceptor _acceptor;
};

#endif /* !SERVERSOCKET_H_ */
