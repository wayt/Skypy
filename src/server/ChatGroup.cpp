#include "ChatGroup.h"
#include "Session.h"
#include "Opcodes.h"
#include "Skypy.h"

ChatGroup::ChatGroup(uint32 id, Session const* owner) :
    _groupId(id), _membersSet()
{
    _membersSet.insert(owner->getId());
    Packet data(SMSG_JOIN_CHAT_GROUP);
    data << uint32(id);
    data << uint32(0);
    owner->send(data);
}

void ChatGroup::addMember(Session const* sess)
{
    if (_membersSet.find(sess->getId()) != _membersSet.end())
        return;

    Packet data(SMSG_CHAT_GROUP_ADD_MEMBER);
    data << uint32(getId());
    sess->buildLoginPacket(data);

    Packet joinData(SMSG_JOIN_CHAT_GROUP);
    joinData << uint32(getId());
    joinData << uint32(_membersSet.size());

    for (std::set<uint32>::const_iterator itr = _membersSet.begin();
            itr != _membersSet.end(); ++itr)
    {
        if (Session const* member = sSkypy->findSession(*itr))
        {
            member->send(data);
            member->buildLoginPacket(joinData);
        }
    }

    _membersSet.insert(sess->getId());
    sess->send(joinData);
}

void ChatGroup::addMessageFrom(Session const* from, std::string const& msg) const
{
    Packet data(SMSG_GROUP_CHAT_TEXT);
    data << uint32(getId());
    data << uint32(from->getId());
    data << msg;

    for (std::set<uint32>::const_iterator itr = _membersSet.begin();
            itr != _membersSet.end(); ++itr)
    {
        if (Session const* member = sSkypy->findSession(*itr))
        {
            std::cout << "FORWARD MESSAGE TO: " << member->getEmail() << std::endl;
            member->send(data);
        }
    }
}
