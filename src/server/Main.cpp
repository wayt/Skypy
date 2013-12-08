#include "SocketMgr.hpp"

int main(int ac, char** av)
{
    SocketMgr mgr;
    if (!mgr.startNetwork(5000, 1))
    {
        std::cerr << "FAIL TO START NETWORK" << std::endl;
        return 1;
    }
    mgr.wait();
    return 0;
}
