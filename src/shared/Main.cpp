#include "SocketMgr.hpp"

class MySockMgr : public SocketMgr
{
public:
    void handleHeaderError(SessionSocket* sock, std::error_code const& error)
    {
        SocketMgr::handleBodyError(sock, error);
        std::cerr << "HEADER ERROR" << std::endl;
    }
    void handleBodyError(SessionSocket* sock, std::error_code const& error)
    {
        SocketMgr::handleBodyError(sock, error);
        std::cerr << "BODY ERROR" << std::endl;
    }
    void handleInvalidHeaderSize(SessionSocket* sock, uint16_t size)
    {
        SocketMgr::handleInvalidHeaderSize(sock, size);
        std::cerr << "INVALIDE HEADER SIZE" << std::endl;
    }
    void handleWriteError(SessionSocket* sock, std::error_code const& error)
    {
        SocketMgr::handleWriteError(sock, error);
        std::cerr << "WRITE ERROR" << std::endl;
    }
};

int main(int ac, char** av)
{
    MySockMgr sock;
    if (!sock.startNetwork(8000, 1))
        std::cerr << "FAIL TO START NETWORK" << std::endl;
    sock.wait();
    return 0;
}
