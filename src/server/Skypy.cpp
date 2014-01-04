#include "Skypy.h"
#include <iostream>
#include "Session.h"
#include "ConfigMgr.h"
#include "SkypyDatabase.h"
#include "ContactMgr.h"
#include "ChatGroupMgr.h"

Skypy::Skypy() : _stopEvent(false), _sessionAddMutex(), _sessionAddList(),
    _sessionDelMutex(), _sessionDelList(), _sessionMap(),
    _networkMgr(), _ac(0), _av(NULL), _timedActionMap()
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

    std::cout << ">> Loading ContactMgr ..." << std::endl;
    sContactMgr->loadFromDb();

    std::cout << ">> Loading ChatGroupMgr ..." << std::endl;
    sChatGroupMgr->loadFromDb();

    std::cout << ">> Clearing online account ..." << std::endl;
    sSkypyDb->execute("UPDATE account SET online = 0");

    std::cout << ">> Loading timed actions ..." << std::endl;
    _loadTimedActions();

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
    std::cout << ">> Saving ContactMgr ..." << std::endl;
    sContactMgr->saveToDb();
    std::cout << ">> Savint ChatGroupMgr ..." << std::endl;
    sChatGroupMgr->saveToDb();

    for (std::map<uint32, Utils::Timer*>::iterator itr = _timedActionMap.begin();
            itr != _timedActionMap.end(); ++itr)
        delete itr->second;
}

void Skypy::update(uint32 diff)
{
    _processAddSession();
    _updateSessions(diff);

    for (std::map<uint32, Utils::Timer*>::iterator itr = _timedActionMap.begin();
            itr != _timedActionMap.end(); ++itr)
    {
        itr->second->update(diff);
        if (itr->second->passed())
        {
            _executeTimedAction(TimedActions(itr->first));
            itr->second->reset();
        }
    }

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
        _sessionMap[(*itr)->getId()] = *itr;
        _handleSessionLogin(*itr);
    }
    _sessionAddList.clear();
}

void Skypy::_processDelSession()
{
    Mutex::ScopLock lock(_sessionDelMutex);
    for (std::list<Session*>::const_iterator itr = _sessionDelList.begin();
            itr != _sessionDelList.end(); ++itr)
    {
        _sessionMap.erase((*itr)->getId());
        _handleSessionLogout(*itr);
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

Session* Skypy::findSession(std::string const& email)
{
    for (std::map<uint32, Session*>::const_iterator itr = _sessionMap.begin();
            itr != _sessionMap.end(); ++itr)
        if (itr->second->getEmail() == email)
            return itr->second;
    return NULL;
}

void Skypy::_handleSessionLogin(Session* sess)
{
    Packet info(SMSG_ACCOUNT_INFO);
    info << uint32(sess->getId());
    info << sess->getName();
    info << sess->getEmail();
    info << sess->getHostAddress();
    sess->send(info);

    try
    {
        std::map<uint32, ContactInfo*> const& contacts = sContactMgr->getContactMap(sess->getId());
        for (std::map<uint32, ContactInfo*>::const_iterator itr = contacts.begin();
            itr != contacts.end(); ++itr)
        if (Session* peer = sSkypy->findSession(itr->first))
            if (peer->hasFriend(sess))
            {
                Packet data(SMSG_CONTACT_LOGIN);
                data << uint32(1);
                sess->buildLoginPacket(data);
                peer->send(data);
            }
    }
    catch (std::exception const&)
    {
    }

    Packet peerData(SMSG_CONTACT_LIST);
    sess->buildFriendListPacket(peerData);
    sess->send(peerData);

    // Send new contact requests to client
    sess->sendContactRequest();

    sChatGroupMgr->handleSessionLogin(sess);
}

void Skypy::_handleSessionLogout(Session* sess)
{
    Packet data(SMSG_CONTACT_LOGOUT);
    data << uint32(1);
    data << uint32(sess->getId());
    sess->broadcastToFriend(data);

    sChatGroupMgr->handleSessionLogout(sess);
}

void Skypy::_loadTimedActions()
{
    _timedActionMap[TM_ACTION_SAVE_CONTACTMGR] = new Utils::Timer(sConfig->getIntDefault("Skypy.ContactMgr.SaveInterval", 60));
    _timedActionMap[TM_ACTION_SAVE_CHATGRPMGR] = new Utils::Timer(sConfig->getIntDefault("Skypy.ChatGroupMgr.SaveInterval", 60));
}

void Skypy::_executeTimedAction(TimedActions action)
{
    switch (action)
    {
        case TM_ACTION_SAVE_CONTACTMGR:
            std::cout << "Saving ContactMgr ..." << std::endl;
            sContactMgr->saveToDb();
            break;
        case TM_ACTION_SAVE_CHATGRPMGR:
            std::cout << "Saving ChatGroupMgr ..." << std::endl;
            sChatGroupMgr->saveToDb();
            break;
    }
}
