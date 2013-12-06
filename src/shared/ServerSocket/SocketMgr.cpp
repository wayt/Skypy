#include "SocketMgr.hpp"

bool SocketMgr::startNetwork(unsigned short port, unsigned int threadCount)
{
    if (threadCount == 0)
    {
        std::cerr << "Error: invalide network thread count: " << threadCount << std::endl;
        return false;
    }

    try {
        _srvSock = new ServerSocket(this, port);

        for (unsigned int i = 0; i < threadCount; ++i)
        {
            _threads.create_thread(
                        [&]() {
                        std::cout << "START THERAD" << std::endl;
                            _service.run();
                            std::cout << "END THREAD" << std::endl;
                        }
                    );
        }

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
}
