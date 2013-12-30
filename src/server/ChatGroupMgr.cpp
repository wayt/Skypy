#include "ChatGroupMgr.h"
#include "ChatGroup.h"

ChatGroupMgr::ChatGroupMgr() :
    _chatGroupMap(), _nextChatGroupId(1)
{}

ChatGroup* ChatGroupMgr::findChatGroup(uint32 id)
{
    std::map<uint32, ChatGroup*>::iterator itr = _chatGroupMap.find(id);
    if (itr != _chatGroupMap.end())
        return itr->second;
    return NULL;
}

ChatGroup const* ChatGroupMgr::findChatGroup(uint32 id) const
{
    std::map<uint32, ChatGroup*>::const_iterator itr = _chatGroupMap.find(id);
    if (itr != _chatGroupMap.end())
        return itr->second;
    return NULL;
}

ChatGroup* ChatGroupMgr::createChatGroup(Session const* owner, Session const* peer)
{
    ChatGroup* chat = new ChatGroup(_nextChatGroupId++, owner);
    chat->addMember(peer);
    _chatGroupMap[chat->getId()] = chat;
    return chat;
}
