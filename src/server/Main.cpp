#include <iostream>
#include "DbConnection.h"
#include "DbWorkerPool.h"
#include "Skypy.h"
#include "Thread.hpp"
#include "Utils.hpp"
#include "ConfigMgr.h"

#define SERVER_SLEEP_TIME 50

void handleSignal(boost::system::error_code const& error, int sig_num)
{
    sSkypy->stopNow();
}

class SkypyRunnable
{
public:
    SkypyRunnable() : _signalService(), _signals(_signalService)
    {
        _signals.add(SIGINT);
        _signals.add(SIGTERM);
    }

    void run()
    {
        _signals.async_wait(&handleSignal);
        Thread signalThread(&_signalService);

        sSkypy->onStartup();

        uint32 prevTime = Utils::getMSTime();
        uint32 currTime = 0;
        uint32 prevSleep = 0;
        while (!sSkypy->getStopEvent())
        {
            currTime = Utils::getMSTime();
            uint32 diff = Utils::getMSTimeDiff(prevTime, currTime);

            sSkypy->update(diff);
            prevTime = currTime;

            if (diff <= SERVER_SLEEP_TIME + prevSleep)
            {
                prevSleep = SERVER_SLEEP_TIME + prevSleep - diff;
                Thread::Sleep(prevSleep);
            }
            else
                prevSleep = 0;
        }

        _signalService.stop();
        signalThread.join();

        sSkypy->onShutdown();
    }

private:
    boost::asio::io_service _signalService;
    boost::asio::signal_set _signals;
};

int main(int ac, char** av)
{

    sSkypy->setAcAv(ac, av);

    SkypyRunnable runn;
    Thread t(&runn);
    t.join();
    return 0;
}
