#include "SocketMgr.hpp"

bool SocketMgr::StartNetwork(std::string const& addr, unsigned short port, unsigned int threadCount)
{
    if (threadCount == 0)
    {
        std::cerr << "Error: invalide network thread count: " << threadCount << std::endl;
        return false;
    }

    try {
        _srvSock = new ServerSocket(this, addr, port);

        for (unsigned int i = 0; i < threadCount; ++i)
        {
            _threads.create_thread(
                        [&]() {
                            _service.run();
                        }
                    );
        }

    } catch (std::exception const& e) {
        std::cerr << "Fail to start network: " << e.what() << std::endl;
        return false;
    }
    return true;
}
