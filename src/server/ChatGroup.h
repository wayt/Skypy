#ifndef CHATGROUP_H_
# define CHATGROUP_H_

#include "SharedDefines.h"
#include <set>
#include <string>

class Session;

class ChatGroup
{
public:
    ChatGroup(uint32 id, Session const* owner);

    uint32 getId() const { return _groupId; }

    void addMember(Session const* sess);
    bool isMember(uint32 id) const { return _membersSet.find(id) != _membersSet.end(); }

    void addMessageFrom(Session const* from, std::string const& msg) const;
private:
    uint32 _groupId;
    std::set<uint32> _membersSet;
};


#endif /* !CHATGROUP_H_ */
