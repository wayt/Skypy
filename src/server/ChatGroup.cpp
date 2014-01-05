#include "ChatGroup.h"
#include "Session.h"
#include "Opcodes.h"
#include "SkypyDatabase.h"

ChatGroupMember::ChatGroupMember(Session const* sess) :
    id(sess->getId()), name(sess->getName()), email(sess->getEmail()), publicIp(sess->getHostAddress()), privateIp(sess->getPrivateAddress()), online(true)
{}

ChatGroupMember::ChatGroupMember(uint32 id, std::string const& name, std::string const& email, std::string const& ipPub, std::string const& ipPriv, bool online) :
    id(id), name(name), email(email), publicIp(ipPub), privateIp(ipPriv), online(online)
{}

ChatGroup::ChatGroup(uint32 id, Session const* owner) :
    _groupId(id), _members(), _createDate(Utils::getTime()), _saveStatus(STATUS_NEW)
{
    ChatGroupMember* member = new ChatGroupMember(owner);
    _members[member->id] = member;

    Packet data(SMSG_JOIN_CHAT_GROUP);
    data << uint32(id);
    data << uint32(0);
    owner->send(data);
}

ChatGroup::ChatGroup(uint32 id, uint32 createDate) :
    _groupId(id), _members(), _createDate(createDate), _saveStatus(STATUS_SAVED)
{
}

void ChatGroup::addMember(Session const* sess)
{
    if (isMember(sess->getId()))
        return;

    ChatGroupMember* member = new ChatGroupMember(sess);

    Packet data(SMSG_CHAT_GROUP_ADD_MEMBER);
    data << uint32(getId());
    buildMemberPacket(data, member);

    Packet joinData(SMSG_JOIN_CHAT_GROUP);
    joinData << uint32(getId());
    joinData << uint32(_members.size());

    for (std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.begin();
            itr != _members.end(); ++itr)
    {
        buildMemberPacket(joinData, itr->second);

        if (Session const* member = sSkypy->findSession(itr->first))
            member->send(data);
    }

    _members[member->id] = member;
    sess->send(joinData);

    if (_saveStatus != STATUS_NEW)
        _saveStatus = STATUS_CHANGED;
}

void ChatGroup::removeMember(Session const* sess)
{
    std::map<uint32, ChatGroupMember*>::iterator itr = _members.find(sess->getId());
    if (itr == _members.end())
        return;
    ChatGroupMember* member = itr->second;
    _members.erase(sess->getId());
    delete member;

    if (_members.empty())
        return;

    Packet data(SMSG_CHAT_GROUP_DEL_MEMBER);
    data << uint32(getId());
    data << uint32(sess->getId());
    broadcastToGroup(data);
}

void ChatGroup::buildMemberPacket(Packet& data, ChatGroupMember const* member)
{
    data << uint32(member->id);
    data << member->name;
    data << member->email;
    data << member->publicIp;
    data << member->privateIp;
    data << uint8(member->online ? 1 : 0);
}

void ChatGroup::addMessageFrom(Session const* from, std::string const& msg) const
{
    Packet data(SMSG_GROUP_CHAT_TEXT);
    data << uint32(getId());
    data << uint32(from->getId());
    data << msg;

    broadcastToGroup(data);
}

void ChatGroup::saveToDb()
{
    switch (_saveStatus)
    {
        case STATUS_NEW:
            sSkypyDb->execute("INSERT INTO chat_group (id, create_date) VALUES ('%u', '%u')", _groupId, _createDate);
            for (std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.begin();
                    itr != _members.end(); ++itr)
                sSkypyDb->execute("INSERT INTO chat_group_member (chat_id, member_id) VALUES ('%u', '%u')", _groupId, itr->first);
            break;
        case STATUS_CHANGED:
            sSkypyDb->execute("UPDATE chat_group SET create_date = '%u' WHERE id = '%u'", _createDate, _groupId);
            sSkypyDb->execute("DELETE FROM chat_group_member WHERE chat_id = '%u'", _groupId);
            for (std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.begin();
                    itr != _members.end(); ++itr)
                sSkypyDb->execute("INSERT INTO chat_group_member (chat_id, member_id) VALUES ('%u', '%u')", _groupId, itr->first);
            break;
        case STATUS_DELETED:
            sSkypyDb->execute("DELETE FROM chat_group WHERE id = '%u'", _groupId);
            sSkypyDb->execute("DELETE FROM chat_group_member WHERE chat_id = '%u'", _groupId);
            return;
        case STATUS_SAVED:
        default:
            break;
    }
    _saveStatus = STATUS_SAVED;
}

void ChatGroup::loadFromDb()
{
    DbResultPtr result = sSkypyDb->query("SELECT cmg.member_id, a.name, a.email FROM chat_group_member cgm LEFT JOIN account a ON cgm.member_id = a.id WHERE cgm.chat_id = '%u'", _groupId);

    while (result->fetch())
    {
        uint32 memberId = (*result)["member_id"]->getValue<uint32>();
        std::string name = (*result)["name"]->getValue();
        std::string email = (*result)["name"]->getValue();
        ChatGroupMember* member = new ChatGroupMember(memberId, name, email, "", "", false);
        _members[memberId] = member;
    }
}

ChatGroupMember* ChatGroup::findMember(uint32 id)
{
    std::map<uint32, ChatGroupMember*>::iterator itr = _members.find(id);
    if (itr != _members.end())
        return itr->second;
    return NULL;
}

ChatGroupMember const* ChatGroup::findMember(uint32 id) const
{
    std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.find(id);
    if (itr != _members.end())
        return itr->second;
    return NULL;
}

void ChatGroup::memberLogin(Session const* member)
{
    ChatGroupMember* __member = findMember(member->getId());
    if (!__member)
        return;
    __member->online = true;
    __member->publicIp = member->getHostAddress();
    __member->privateIp = member->getPrivateAddress();

    Packet upd(SMSG_CHAT_GROUP_UPD_MEMBER);
    upd << uint32(getId());
    buildMemberPacket(upd, __member);
    broadcastToGroup(upd, __member->id);

    Packet data(SMSG_JOIN_CHAT_GROUP);
    data << uint32(getId());
    data << uint32(_members.size());

    for (std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.begin();
            itr != _members.end(); ++itr)
        buildMemberPacket(data, itr->second);

    member->send(data);
}

void ChatGroup::memberLogout(uint32 id)
{
    ChatGroupMember* member = findMember(id);
    if (!member)
        return;
    member->online = false;
    member->publicIp = "";
    member->privateIp = "";

    Packet upd(SMSG_CHAT_GROUP_UPD_MEMBER);
    upd << uint32(getId());
    buildMemberPacket(upd, member);
    broadcastToGroup(upd, member->id);
}

void ChatGroup::broadcastToGroup(Packet const& pkt, uint32 except) const
{
    for (std::map<uint32, ChatGroupMember*>::const_iterator itr = _members.begin();
            itr != _members.end(); ++itr)
        if (itr->first != except)
            if (Session const* member = sSkypy->findSession(itr->first))
                member->send(pkt);
}
