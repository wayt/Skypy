#ifndef CHATGROUPMGR_H_
# define CHATGROUPMGR_H_

#include "Singleton.hpp"
#include <map>
#include "ChatGroup.h"

class ChatGroupMgr : public Singleton<ChatGroupMgr>
{
public:
    ChatGroupMgr();
    ChatGroup* findChatGroup(uint32 id);
    ChatGroup const* findChatGroup(uint32 id) const;
    ChatGroup* createChatGroup(Session const* owner, Session const* firstPeer);
    void deleteChatGroup(ChatGroup* group);

    void saveToDb();
    void loadFromDb();

    void handleSessionLogin(Session const* sess);
    void handleSessionLogout(Session const* sess);

private:
    std::map<uint32, ChatGroup*> _chatGroupMap;
    uint32 _nextChatGroupId;
};

#define sChatGroupMgr ChatGroupMgr::instance()

#endif /* !CHATGROUPMGR_H_ */
