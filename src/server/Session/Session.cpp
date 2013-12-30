#include "Session.h"
#include "SessionSocket.h"
#include "Skypy.h"
#include "SipManager.h"
#include "SkypyDatabase.h"
#include <algorithm>
#include "AuthWorker.h"
#include "ContactMgr.h"
#include "ChatGroupMgr.h"

Session::Session(SessionSocket* sock, std::string const& email, std::string const& privateIp) : _id(0), _socket(sock), _packetQueue(),
    _logout(false), _name(email), _email(email), _privateIp(privateIp)
{
    std::size_t found = _email.find('@');
    if (found != std::string::npos)
        _name = _email.substr(0, found);
}

std::string const& Session::getRemoteAddess() const
{
    return _socket->getRemoteAddress();
}

void Session::logout()
{
    delete _socket;
    _socket = NULL;
    _logout = true;
    sSkypyDb->execute("UPDATE account SET online = 0 WHERE id = '%u'", getId());
    sSkypy->delSession(this);
}

bool Session::loadFromDb()
{
    _loadAccountInfo();

    sSkypyDb->execute("UPDATE account SET online = 1 WHERE id = '%u'", getId());

    return true;
}

void Session::_loadAccountInfo()
{
    DbResultPtr result = sSkypyDb->query("SELECT id, name FROM account WHERE email = '%s'", _email.c_str());
    if (!result->fetch())
        throw std::runtime_error("Fail to load account info");

    _id = (*result)["id"]->getValue<uint32>();
    _name = (*result)["name"]->getValue();
}

bool Session::hasFriend(uint32 id) const
{
    return sContactMgr->hasFriend(getId(), id);
}

void Session::addFriend(ContactInfo* info)
{
    sContactMgr->addFriend(getId(), info);
}

void Session::broadcastToFriend(Packet const& pkt) const
{
    try
    {
        std::map<uint32, ContactInfo*> const& contacts = sContactMgr->getContactMap(getId());
        for (std::map<uint32, ContactInfo*>::const_iterator itr = contacts.begin();
            itr != contacts.end(); ++itr)
        if (Session* peer = sSkypy->findSession(itr->first))
            if (peer->hasFriend(this))
            {
                std::cout << "BROAD TO FRIEND FROM " << getEmail() << " TO " << peer->getEmail() << std::endl;
                peer->send(pkt);
            }
    }
    catch (std::exception const&)
    {
    }
}

void Session::buildOnlineFriendPacket(Packet& pkt) const
{
    uint32 count = 0;
    uint16 holder = pkt.insertPlaceholder<uint32>();

    try
    {
        std::map<uint32, ContactInfo*> const& contacts = sContactMgr->getContactMap(getId());
        for (std::map<uint32, ContactInfo*>::const_iterator itr = contacts.begin();
            itr != contacts.end(); ++itr)
        if (Session* peer = sSkypy->findSession(itr->first))
            if (peer->hasFriend(this))
            {
                peer->buildLoginPacket(pkt, this);
                ++count;
                std::cout << "PEER: " << peer->getId() << std::endl << "NAME: " << peer->getName() << std::endl << "MAIL: " << peer->getEmail() << std::endl;
            }
    }
    catch (std::exception const&)
    {
        count = 0;
    }
    pkt.insert<uint32>(count, holder);
}

void Session::buildLoginPacket(Packet& pkt, Session const* to) const
{
    pkt << uint32(getId());
    pkt << getName();
    pkt << getEmail();
    pkt << getHostAddress();
    pkt << getPrivateAddress();
}

void Session::update(uint32 diff)
{
    if (!_socket || _logout)
        return;

    uint32 size = _packetQueue.size();
    for (uint32 i = 0; i < size && !_packetQueue.empty() && _socket && !_logout; ++i)
    {
        Packet* pkt = _packetQueue.get();
        if (!pkt)
            continue;

        OpcodeMgr::OpcodeDefinition const* opcodehandler = OpcodeMgr::getOpcodeDefinition(pkt->getOpcode());

        if (!opcodehandler)
        {
            std::cerr << "Error: receiv an unknow opcode : " << std::hex << pkt->getOpcode() << std::endl;
            delete pkt;
            continue;
        }
        if (opcodehandler->func)
            (this->*opcodehandler->func)(*pkt);
        delete pkt;
    }
}

void Session::handlePacketInput(Packet& pkt)
{
    Packet* newPacket = new Packet(pkt);
    _packetQueue.add(newPacket);
}

void Session::send(Packet const& pkt)
{
    ON_NETWORK_DEBUG(
            std::cout << "Network: Session::Send" << std::endl;
            pkt.dumpHex();
    );
    if (_socket && !_logout)
        _socket->send(pkt);
}

std::string const& Session::getHostAddress() const
{
    return _socket->getRemoteAddress();
}

void Session::sendContactRequest()
{
    try
    {
        std::map<uint32, ContactRequest*> const& requests = sContactMgr->getContactRequestFor(getId());

        for (std::map<uint32, ContactRequest*>::const_iterator itr = requests.begin();
                itr != requests.end(); ++itr)
        {
            Packet data(SMSG_ADD_CONTACT_REQUEST);
            data << uint32(itr->second->requestId);
            data << itr->second->fromName;
            data << itr->second->fromEmail;
            send(data);
        }
    }
    catch (std::exception const&)
    {
    }
}

void Session::friendLogin(Session* sess)
{
    Packet data(SMSG_CONTACT_LOGIN);
    data << uint32(1);
    sess->buildLoginPacket(data, this);
    send(data);
}

void Session::handleSipPacket(Packet& pkt)
{
    std::string cmd;
    std::string senderEmail;
    uint32 senderId;
    std::string senderIp;
    uint32 senderPort;
    std::string destEmail;
    uint32 destId;
    std::string destIp;
    uint32 destPort;

    std::cout << "SIP REQUESTPACKET RECEIVED" << std::endl;
    pkt >> cmd;
    pkt >> senderEmail >> senderId >> senderIp >> senderPort;
    pkt >> destEmail >> destId >> destIp >> destPort;
    sSipManager->sendSipResponse(this, 100, cmd, senderEmail, senderId, senderIp, senderPort, destEmail, destId, destIp, destPort);
    Session* peer = sSkypy->findSession(destId);
    if (!peer)
    {
        std::cout << "Peer not found" << std::endl;
        sSipManager->sendSipResponse(this, 404, cmd, senderEmail, senderId, senderIp, senderPort, destEmail, destId, destIp, destPort);
        return;
    }
    else
    {
        sSipManager->forwardSip(peer, pkt);
        sSipManager->sendSipResponse(this, 180, cmd, senderEmail, senderId, senderIp, senderPort, destEmail, destId, destIp, destPort);
    }
}

void Session::handleSipRespond(Packet& pkt)
{
    uint32 code;
    std::string cmd;
    std::string senderEmail;
    uint32 senderId;
    std::string senderIp;
    uint32 senderPort;
    std::string destEmail;
    uint32 destId;
    std::string destIp;
    uint32 destPort;

    std::cout << "SIP REQUESTPACKET RECEIVED" << std::endl;
    pkt >> code >> cmd;
    pkt >> senderEmail >> senderId >> senderIp >> senderPort;
    pkt >> destEmail >> destId >> destIp >> destPort;

    if (Session* sess = sSkypy->findSession(senderId))
        sSipManager->forwardSip(sess, pkt);
}

void Session::handleChatText(Packet& pkt)
{
    uint32 peerId;
    std::string msg;
    pkt >> peerId >> msg;

    Session* peer = sSkypy->findSession(peerId);
    if (!peer)
        return;

    Packet data(SMSG_CHAT_TEXT);
    data << uint32(getId());
    data << msg;
    peer->send(data);
}

void Session::handleSearchNewContact(Packet& pkt)
{
    std::string word;
    pkt >> word;

    std::replace(word.begin(), word.end(), '%', 'a');
    std::replace(word.begin(), word.end(), '_', 'a');
    if (word.length() < 2)
        return;

    DbResultPtr result = sSkypyDb->query("SELECT id, name, email, online FROM account WHERE (email LIKE '%%%s%%@%%' OR name like '%%%s%%') AND email != '%s' LIMIT 25", word.c_str(), word.c_str(), _email.c_str());

    Packet data(SMSG_SEARCH_CONTACT_RESULT);
    uint32 count = 0;
    uint16 holder = data.insertPlaceholder<uint32>();
    while (result->fetch())
    {
        uint32 id = (*result)["id"]->getValue<uint32>();
        std::string name = (*result)["name"]->getValue();
        std::string email = (*result)["email"]->getValue();
        bool online = (*result)["online"]->getValue<bool>();

        if (hasFriend(id) || sContactMgr->hasContactRequest(getId(), id))
        {
            std::cout << _email << " has friend " << email << std::endl;
            continue;
        }

        data << name;
        data << email;
        data << uint8(online);
        std::cout << "NAME: " << email << " - " << uint32(online) << std::endl;
        ++count;
    }
    data.insert<uint32>(count, holder);
    send(data);
}

void Session::handleAddContactRequest(Packet& pkt)
{
    std::string email;
    pkt >> email;

    if (!AuthWorker::isValidEmail(email))
        return;

    Session* sess = sSkypy->findSession(email);
    uint32 dest_id = (sess ? sess->getId() : 0);

    if (!sess)
    {
        DbResultPtr result = sSkypyDb->query("SELECT id FROM account WHERE email = '%s'", email.c_str());
        if (!result->fetch())
            return;

        dest_id = (*result)["id"]->getValue<uint32>();
    }

    bool hasFriend = sContactMgr->hasFriend(dest_id, getId());
    if (hasFriend)
    {
        uint32 time = Utils::getTime();
        addFriend(new ContactInfo(dest_id, time));
        if (sess)
        {
            friendLogin(sess);
            sess->friendLogin(this);
        }
    }
    else
    {
        // Check existing request
        ContactRequest* req = new ContactRequest(0, getId(), getName(), getEmail(), dest_id, Utils::getTime());
        if (!sContactMgr->addContactRequest(req))
            return;

        if (sess)
        {
            Packet data(SMSG_ADD_CONTACT_REQUEST);
            data << uint32(req->requestId);
            data << getName();
            data << getEmail();
            sess->send(data);
        }
    }
}

void Session::handleAddContactResponse(Packet& pkt)
{
    uint32 reqId;
    bool accept;
    pkt >> reqId >> accept;

    ContactRequest* request = sContactMgr->getContactRequest(reqId);
    if (!request || request->destId != getId())
        return;

    if (accept)
    {
        uint32 time = Utils::getTime();

        std::cout << getEmail() << " accept contact request " << reqId << " from " << request->fromId << " at " << time << std::endl;

        addFriend(new ContactInfo(request->fromId, time));
        sContactMgr->addFriend(request->fromId, new ContactInfo(request->destId, time));

        if (Session* sess = sSkypy->findSession(request->fromId))
        {
            friendLogin(sess);
            sess->friendLogin(this);
        }
    }
    else
        std::cout << getEmail() << " refuse contact request " << reqId << std::endl;

    sContactMgr->deleteContactRequest(request);
}

void Session::handleGetAccountInfo(Packet& pkt)
{
    std::cout << " CLient " << getEmail() << " request account info" << std::endl;
    Packet info(SMSG_ACCOUNT_INFO);
    info << uint32(getId());
    info << getName();
    info << getEmail();
    info << getRemoteAddess();
    send(info);
}

void Session::handleChatGroupGetAvailable(Packet& pkt)
{
    uint32 chatId;
    pkt >> chatId;

    if (ChatGroup* chat = sChatGroupMgr->findChatGroup(chatId))
    {

    }
    else // New chat
    {

    }



}
