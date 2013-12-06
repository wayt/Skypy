#include "SocketMgr.hpp"

int main(int ac, char** av)
{
    SocketMgr sock;
    if (!sock.startNetwork(8000, 1))
        std::cerr << "FAIL TO START NETWORK" << std::endl;
    sock.wait();
    return 0;
}
