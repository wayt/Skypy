#include "Skypy.h"
#include <iostream>
#include "Session.h"
#include "ConfigMgr.h"
#include "SkypyDatabase.h"

Skypy::Skypy() : _stopEvent(false), _sessionAddMutex(), _sessionAddList(),
    _sessionDelMutex(), _sessionDelList(), _sessionMap(),
    _networkMgr(), _ac(0), _av(NULL)
{

}

void Skypy::onStartup()
{
    std::cout << "*****************************************" << std::endl;
    std::cout << "*     _____ _                           *" << std::endl;
    std::cout << "*    /  ___| |                          *" << std::endl;
    std::cout << "*    \\ `--.| | ___   _ _ __  _   _      *" << std::endl;
    std::cout << "*     `--. \\ |/ / | | | '_ \\| | | |     *" << std::endl;
    std::cout << "*    /\\__/ /   <| |_| | |_) | |_| |     *" << std::endl;
    std::cout << "*    \\____/|_|\\_\\\\__, | .__/ \\__, |     *" << std::endl;
    std::cout << "*                 __/ | |     __/ |     *" << std::endl;
    std::cout << "*                |___/|_|    |___/      *" << std::endl;
    std::cout << "*                        Coupyright <3  *" << std::endl;
    std::cout << "*****************************************" << std::endl << std::endl;

    std::cout << ">> Load configuration ..." << std::endl;
    sConfig->setConfigFile((_ac > 1 ? _av[1] : DEFAULT_CONFIG_FILE));
    if (!sConfig->loadConfig())
    {
        std::cerr << "Fail to load config file \"" << sConfig->getConfigFile() << "\"" << std::endl;
        stopNow();
        return;
    }

    std::cout << ">> Starting Database pool ..." << std::endl;
    if (!sSkypyDb->initialize())
    {
        std::cerr << "Fail to init MySQL " << sSkypyDb->getLastError() << std::endl;
        stopNow();
        return;
    }

    std::cout << ">> Starting network ..." << std::endl;
    if (!_networkMgr.startNetwork(
                sConfig->getIntDefault("Network.ListenPort", 5000),
                sConfig->getIntDefault("Network.ThreadCount", 1)
                ))
    {
        stopNow();
        return;
    }
}

void Skypy::onShutdown()
{
    std::cout << "Stopping Skypy server..." << std::endl;
}

void Skypy::update(uint32 diff)
{
    _processAddSession();
    _updateSessions(diff);
    _processDelSession();
}

void Skypy::addSession(Session* sess)
{
    Mutex::ScopLock lock(_sessionAddMutex);
    _sessionAddList.push_back(sess);
}

void Skypy::delSession(Session* sess)
{
    Mutex::ScopLock lock(_sessionDelMutex);

    _sessionDelList.push_back(sess);
}

void Skypy::_processAddSession()
{
    Mutex::ScopLock lock(_sessionAddMutex);
    for (std::list<Session*>::const_iterator itr = _sessionAddList.begin();
            itr != _sessionAddList.end(); ++itr)
    {
        Session* sess = *itr;

        Packet selfData(SMSG_CONTACT_LOGIN);
        selfData << uint32(1);
        selfData << uint32(sess->getId());
        selfData << sess->getName();
        selfData << sess->getEmail();
        for (std::map<uint32, Session*>::const_iterator itr2 = _sessionMap.begin();
                itr2 != _sessionMap.end(); ++itr2)
        {
            Packet data(SMSG_CONTACT_LOGIN);
            data << uint32(1);
            data << uint32(itr2->second->getId());
            data << itr2->second->getName();
            data << itr2->second->getEmail();
            sess->send(data);
            itr2->second->send(selfData);
        }



        _sessionMap[(*itr)->getId()] = *itr;
    }
    _sessionAddList.clear();
}

void Skypy::_processDelSession()
{
    Mutex::ScopLock lock(_sessionDelMutex);
    for (std::list<Session*>::const_iterator itr = _sessionDelList.begin();
            itr != _sessionDelList.end(); ++itr)
    {
        Session* sess = *itr;

        Packet data(SMSG_CONTACT_LOGOUT);
        data << uint32(1);
        data << uint32(sess->getId());
        for (std::map<uint32, Session*>::const_iterator itr2 = _sessionMap.begin();
                itr2 != _sessionMap.end(); ++itr2)
            itr2->second->send(data);



        _sessionMap.erase((*itr)->getId());
        delete *itr;
    }
    _sessionDelList.clear();
}

void Skypy::_updateSessions(uint32 diff)
{
    for (std::map<uint32, Session*>::const_iterator itr = _sessionMap.begin();
            itr != _sessionMap.end(); ++itr)
        if (!itr->second->isLogout())
            itr->second->update(diff);
}

Session* Skypy::findSession(uint32 id)
{
    std::map<uint32, Session*>::const_iterator itr = _sessionMap.find(id);
    if (itr == _sessionMap.end())
        return NULL;
    return itr->second;
}
