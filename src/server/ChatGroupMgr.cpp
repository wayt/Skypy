#include "ChatGroupMgr.h"
#include "ChatGroup.h"
#include "SkypyDatabase.h"

ChatGroupMgr::ChatGroupMgr() :
    _chatGroupMap(), _nextChatGroupId(0)
{}

ChatGroup* ChatGroupMgr::findChatGroup(uint32 id)
{
    std::map<uint32, ChatGroup*>::iterator itr = _chatGroupMap.find(id);
    if (itr != _chatGroupMap.end() && itr->second->getSaveStatus() != STATUS_DELETED)
        return itr->second;
    return NULL;
}

ChatGroup const* ChatGroupMgr::findChatGroup(uint32 id) const
{
    std::map<uint32, ChatGroup*>::const_iterator itr = _chatGroupMap.find(id);
    if (itr != _chatGroupMap.end() && itr->second->getSaveStatus() != STATUS_DELETED)
        return itr->second;
    return NULL;
}

ChatGroup* ChatGroupMgr::createChatGroup(Session const* owner, Session const* peer)
{
    ChatGroup* chat = new ChatGroup(++_nextChatGroupId, owner);
    chat->addMember(peer);
    _chatGroupMap[chat->getId()] = chat;
    return chat;
}

void ChatGroupMgr::deleteChatGroup(ChatGroup* group)
{
    group->setDeleted();
}

void ChatGroupMgr::saveToDb()
{
    for (std::map<uint32, ChatGroup*>::iterator itr = _chatGroupMap.begin();
            itr != _chatGroupMap.end();)
    {
        itr->second->saveToDb();
        if (itr->second->getSaveStatus() == STATUS_DELETED)
        {
            ChatGroup* chat = itr->second;
            _chatGroupMap.erase(itr++);
            delete chat;
        }
        else
            ++itr;
    }
}

void ChatGroupMgr::loadFromDb()
{
    DbResultPtr result = sSkypyDb->query("SELECT id, create_date FROM chat_group");

    uint32 count = 0;
    while (result->fetch())
    {
        uint32 id = (*result)["id"]->getValue<uint32>();
        uint32 createDate = (*result)["create_date"]->getValue<uint32>();

        ChatGroup* chat = new ChatGroup(id, createDate);
        chat->loadFromDb();
        _chatGroupMap[chat->getId()] = chat;

        if (id > _nextChatGroupId)
            _nextChatGroupId = id;

        ++count;
    }

    std::cout << "Loaded " << count << " chats groups" << std::endl;
}

void ChatGroupMgr::handleSessionLogin(Session const* sess)
{
    for (std::map<uint32, ChatGroup*>::iterator itr = _chatGroupMap.begin();
            itr != _chatGroupMap.end(); ++itr)
    {
        if (itr->second->isMember(sess->getId()))
            itr->second->memberLogin(sess);
    }
}

void ChatGroupMgr::handleSessionLogout(Session const* sess)
{
    for (std::map<uint32, ChatGroup*>::iterator itr = _chatGroupMap.begin();
            itr != _chatGroupMap.end(); ++itr)
    {
        if (itr->second->isMember(sess->getId()))
            itr->second->memberLogout(sess->getId());
    }
}


