#ifndef CONTACTMGR_H_
# define CONTACTMGR_H_

#include "Singleton.hpp"
#include "SharedDefines.h"
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

enum SaveStatus
{
    STATUS_NEW      = 0,
    STATUS_SAVED    = 1,
    STATUS_CHANGED  = 2,
    STATUS_DELETED  = 3,
};

struct ContactInfo
{
    ContactInfo(uint32 id, std::string const& name, std::string const& email, uint32 date, SaveStatus status = STATUS_SAVED);
    uint32 id;
    std::string name;
    std::string email;
    uint32 date;
    SaveStatus saveStatus;
};

struct ContactRequest
{
    ContactRequest(uint32 requestId, uint32 fromId, std::string const& fromName, std::string const& fromEmail, uint32 destId, uint32 date, SaveStatus status = STATUS_SAVED);
    uint32 requestId;
    uint32 fromId;
    std::string fromName;
    std::string fromEmail;
    uint32 destId;
    uint32 date;
    SaveStatus saveStatus;
};

class ContactMgr : public Singleton<ContactMgr>
{
public:
    ContactMgr();

    void loadFromDb();
    void saveToDb();

    inline std::map<uint32, ContactInfo*> const& getContactMap(uint32 account) const
    {
        std::map<uint32, std::map<uint32, ContactInfo*>>::const_iterator itr = _contacsMap.find(account);
        if (itr == _contacsMap.end())
            throw std::runtime_error("No contact");
        return itr->second;
    }

    bool hasFriend(uint32 account, uint32 with) const;
    void addFriend(uint32 account, ContactInfo* info);

    bool addContactRequest(ContactRequest* req);
    bool hasContactRequest(uint32 from, uint32 to) const;
    ContactRequest* getContactRequest(uint32 id);
    ContactRequest const* getContactRequest(uint32 id) const;
    void deleteContactRequest(uint32 id);
    void deleteContactRequest(ContactRequest* req) { req->saveStatus = STATUS_DELETED; }

    inline std::map<uint32, ContactRequest*> const& getContactRequestFor(uint32 account) const
    {
        std::map<uint32, std::map<uint32, ContactRequest*>>::const_iterator itr = _contactRequestsMap.find(account);
        if (itr == _contactRequestsMap.end())
            throw std::runtime_error("No request");
        return itr->second;
    }

private:
    std::map<uint32, std::map<uint32, ContactInfo*>> _contacsMap;
    std::map<uint32, std::map<uint32, ContactRequest*>> _contactRequestsMap;
    uint32 _lastRequestId;
};

#define sContactMgr ContactMgr::instance()

#endif /* !CONTACTMGR_H_ */
