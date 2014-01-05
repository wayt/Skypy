#ifndef CHATGROUP_H_
# define CHATGROUP_H_

#include "SharedDefines.h"
#include <set>
#include <string>
#include "Skypy.h"

class Session;

struct ChatGroupMember
{
    ChatGroupMember(Session const* sess);
    ChatGroupMember(uint32 id, std::string const& name, std::string const& email, std::string const& ipPub, std::string const& ipPriv, bool online);

    uint32 id;
    std::string name;
    std::string email;
    std::string publicIp;
    std::string privateIp;
    bool online;
};

class ChatGroup
{
public:
    ChatGroup(uint32 id, Session const* owner);
    ChatGroup(uint32 id, uint32 createDate);

    uint32 getId() const { return _groupId; }

    void addMember(Session const* sess);
    void removeMember(Session const* sess);
    bool isMember(uint32 id) const { return _members.find(id) != _members.end(); }
    void memberLogin(Session const* member);
    void memberLogout(uint32 id);
    ChatGroupMember* findMember(uint32 id);
    ChatGroupMember const* findMember(uint32 id) const;
    bool isEmpty() const { return _members.size() < 2; }
    void setDeleted() { _saveStatus = STATUS_DELETED; }

    void addMessageFrom(Session const* from, std::string const& msg) const;

    void saveToDb();
    void loadFromDb();
    SaveStatus getSaveStatus() const { return _saveStatus; }

    static void buildMemberPacket(Packet& data, ChatGroupMember const* member);

    void broadcastToGroup(Packet const& pkt, uint32 except = 0) const;
private:
    uint32 _groupId;
    std::map<uint32, ChatGroupMember*> _members;
    uint32 _createDate;
    SaveStatus _saveStatus;
};


#endif /* !CHATGROUP_H_ */
