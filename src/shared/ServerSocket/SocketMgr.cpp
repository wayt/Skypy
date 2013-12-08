#include <functional>
#include "SocketMgr.hpp"
#include "ServerSocket.hpp"

bool SocketMgr::startNetwork(unsigned short port, unsigned int threadCount)
{
    if (threadCount == 0)
    {
        std::cerr << "Error: invalide network thread count: " << threadCount << std::endl;
        return false;
    }

    try {
        _srvSock = new ServerSocket(this, port);
        _thread = new Thread(_service);
    } catch (std::exception const& e) {
        std::cerr << "Fail to start network: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void SocketMgr::shutdown()
{
    if (_srvSock)
        _srvSock->shutdown();

    _service.stop();

    if (_thread)
        _thread->join();
}
