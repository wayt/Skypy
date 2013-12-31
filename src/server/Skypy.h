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

enum TimedActions
{
    TM_ACTION_SAVE_CONTACTMGR       = 0,
    TM_ACTION_SAVE_CHATGRPMGR       = 1,
};

class Skypy : public Singleton<Skypy>
{
public:
    Skypy();
    void setAcAv(int ac, char** av) { _ac = ac; _av = av; }
    void onStartup();
    void onShutdown();

    void update(uint32 diff);

    bool getStopEvent() { return _stopEvent.load(); }
    void stopNow() { _stopEvent = true; }

    void addSession(Session* sess);
    void delSession(Session* sess);

    Session* findSession(uint32 id);
    Session* findSession(std::string const& email);

private:
    void _processAddSession();
    void _processDelSession();
    void _updateSessions(uint32 diff);
    void _handleSessionLogin(Session* sess);
    void _handleSessionLogout(Session* sess);

    void _loadTimedActions();
    void _executeTimedAction(TimedActions action);

    std::atomic<bool> _stopEvent;
    Mutex _sessionAddMutex;
    std::list<Session*> _sessionAddList;
    Mutex _sessionDelMutex;
    std::list<Session*> _sessionDelList;
    std::map<uint32, Session*> _sessionMap;
    SocketMgr _networkMgr;
    int _ac;
    char** _av;
    std::map<uint32, Utils::Timer*> _timedActionMap;
};

#define sSkypy Skypy::instance()

#endif /* !SKYPY_H_ */
