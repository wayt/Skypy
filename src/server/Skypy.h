#ifndef SKYPY_H_
# define SKYPY_H_

#include <atomic>
#include "Singleton.hpp"
#include "SharedDefines.h"
#include "Mutex.hpp"
#include <list>
#include <map>
#include "SocketMgr.h"

class Session;

class Skypy : public Singleton<Skypy>
{
public:
    Skypy();
    void onStartup();
    void onShutdown();

    void update(uint32 diff);

    bool getStopEvent() { return _stopEvent.load(); }
    void stopNow() { _stopEvent = true; }

    void addSession(Session* sess);
    void delSession(Session* sess);
private:
    void _processAddSession();
    void _processDelSession();
    void _updateSessions(uint32 diff);

    std::atomic<bool> _stopEvent;
    Mutex _sessionAddMutex;
    std::list<Session*> _sessionAddList;
    Mutex _sessionDelMutex;
    std::list<Session*> _sessionDelList;
    std::map<uint32, Session*> _sessionMap;
    SocketMgr _networkMgr;
};

#define sSkypy Skypy::instance()

#endif /* !SKYPY_H_ */
