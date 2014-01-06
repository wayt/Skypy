#include "ContactMgr.h"
#include "SkypyDatabase.h"

ContactInfo::ContactInfo(uint32 id, std::string const& name, std::string const& email, uint32 date, SaveStatus status) :
    id(id), name(name), email(email), date(date), saveStatus(status)
{
}

ContactRequest::ContactRequest(uint32 requestId, uint32 fromId, std::string const& fromName, std::string const& fromEmail, uint32 destId, uint32 date, SaveStatus status) :
    requestId(requestId), fromId(fromId), fromName(fromName), fromEmail(fromEmail), destId(destId), date(date), saveStatus(status)
{
}

ContactMgr::ContactMgr() :
    _contacsMap(), _contactRequestsMap(), _lastRequestId(0)
{
}

void ContactMgr::loadFromDb()
{
    DbResultPtr result = sSkypyDb->query("SELECT af.account_id, af.friend_id, af.date, a.name, a.email FROM account_friends af LEFT JOIN account a ON af.friend_id = a.id ORDER BY af.account_id");

    uint32 contactCount = 0;
    while (result->fetch())
    {
        uint32 account = (*result)["account_id"]->getValue<uint32>();
        uint32 friendId = (*result)["friend_id"]->getValue<uint32>();
        std::string name = (*result)["name"]->getValue();
        std::string email = (*result)["email"]->getValue();
        uint32 date = (*result)["date"]->getValue<uint32>();

        _contacsMap[account][friendId] = new ContactInfo(friendId, name, email, date, STATUS_SAVED);
        ++contactCount;
    }

    result = sSkypyDb->query("SELECT cr.request_id, cr.dest_id, cr.from_id, cr.date, a.name, a.email FROM contact_requests cr LEFT JOIN account a ON cr.from_id = a.id");

    uint32 requestCount = 0;
    while (result->fetch())
    {
        uint32 id = (*result)["request_id"]->getValue<uint32>();
        uint32 destId = (*result)["dest_id"]->getValue<uint32>();
        uint32 fromId = (*result)["from_id"]->getValue<uint32>();
        std::string fromName = (*result)["name"]->getValue();
        std::string fromEmail = (*result)["email"]->getValue();
        uint32 date = (*result)["date"]->getValue<uint32>();

        _contactRequestsMap[destId][fromId] = new ContactRequest(id, fromId, fromName, fromEmail, destId, date, STATUS_SAVED);
        ++requestCount;

        if (id > _lastRequestId)
            _lastRequestId = id;
    }

    std::cout << "loaded " << contactCount << " contact relations and " << requestCount << " contact requests" << std::endl;
}

void ContactMgr::saveToDb()
{
    for (std::map<uint32, std::map<uint32, ContactInfo*>>::iterator __itr =  _contacsMap.begin();
    __itr != _contacsMap.end(); ++__itr)
    {
        for (std::map<uint32, ContactInfo*>::iterator itr = __itr->second.begin();
                itr != __itr->second.end();)
        {
            switch (itr->second->saveStatus)
            {
                case STATUS_NEW:
                    sSkypyDb->execute("INSERT INTO account_friends (account_id, friend_id, date) VALUES ('%u', '%u', '%u')", __itr->first, itr->second->id, itr->second->date);
                    break;
                case STATUS_CHANGED:
                    sSkypyDb->execute("UPDATE account_friends SET date = '%u' WHERE account_id = '%u' AND friend_id = '%u'", itr->second->date, __itr->first, itr->second->id);
                    break;
                case STATUS_DELETED:
                    {
                        sSkypyDb->execute("DELETE FROM account_friends WHERE account_id = '%u' AND friend_id = '%u'", __itr->first, itr->second->id);
                        ContactInfo* info = itr->second;
                        __itr->second.erase(itr++);
                        delete info;
                        continue;
                    }
                case STATUS_SAVED:
                default:
                    break;
            }
            itr->second->saveStatus = STATUS_SAVED;
            ++itr;
        }
    }

    for (std::map<uint32, std::map<uint32, ContactRequest*>>::iterator __itr = _contactRequestsMap.begin();
            __itr != _contactRequestsMap.end(); ++__itr)
    {
        for (std::map<uint32, ContactRequest*>::iterator itr = __itr->second.begin();
                itr != __itr->second.end();)
        {
            switch (itr->second->saveStatus)
            {
                case STATUS_NEW:
                    sSkypyDb->execute("INSERT INTO contact_requests (request_id, dest_id, from_id, date) VALUES ('%u', '%u', '%u', '%u')", itr->second->requestId, itr->second->destId, itr->second->fromId, itr->second->date);
                    break;
                case STATUS_CHANGED:
                    sSkypyDb->execute("UPDATE contact_requests SET date = '%u' WHERE request_id = '%u'", itr->second->date, itr->second->requestId);
                    break;
                case STATUS_DELETED:
                    {
                        sSkypyDb->execute("DELETE FROM contact_requests WHERE request_id = '%u'", itr->second->requestId);
                        ContactRequest* req = itr->second;
                        __itr->second.erase(itr++);
                        delete req;
                        continue;
                    }
                case STATUS_SAVED:
                default:
                    break;
            }
            itr->second->saveStatus = STATUS_SAVED;
            ++itr;
        }
    }
}
bool ContactMgr::hasFriend(uint32 account, uint32 with) const
{
    std::map<uint32, std::map<uint32, ContactInfo*>>::const_iterator itr = _contacsMap.find(account);
    if (itr == _contacsMap.end())
        return false;

    std::map<uint32, ContactInfo*>::const_iterator itr2 = itr->second.find(with);
    if (itr2 == itr->second.end())
        return false;
    return itr2->second->saveStatus != STATUS_DELETED;
}

void ContactMgr::addFriend(uint32 account, ContactInfo* info)
{
    if (hasFriend(account, info->id))
    {
        delete info;
        return;
    }
    _contacsMap[account][info->id] = info;
    info->saveStatus = STATUS_NEW;
}

void ContactMgr::delFriend(uint32 account, uint32 peer)
{
    std::map<uint32, ContactInfo*>::iterator itr = _contacsMap[account].find(peer);
    if (itr == _contacsMap[account].end())
        return;

    itr->second->saveStatus = STATUS_DELETED;

}

bool ContactMgr::addContactRequest(ContactRequest* req)
{
    if (hasContactRequest(req->fromId, req->destId))
    {
        delete req;
        return false;
    }
    _contactRequestsMap[req->destId][req->fromId] = req;
    req->saveStatus = STATUS_NEW;
    if (req->requestId == 0)
        req->requestId = ++_lastRequestId;
    return true;
}

bool ContactMgr::hasContactRequest(uint32 from, uint32 to) const
{
    std::map<uint32, std::map<uint32, ContactRequest*>>::const_iterator itr = _contactRequestsMap.find(to);
    if (itr == _contactRequestsMap.end())
        return false;

    std::map<uint32, ContactRequest*>::const_iterator itr2 = itr->second.find(from);
    if (itr2 == itr->second.end())
        return false;
    return true;
}

ContactRequest* ContactMgr::getContactRequest(uint32 id)
{
    for (std::map<uint32, std::map<uint32, ContactRequest*>>::iterator __itr = _contactRequestsMap.begin();
            __itr != _contactRequestsMap.end(); ++__itr)
    {
        for (std::map<uint32, ContactRequest*>::iterator itr = __itr->second.begin();
                itr != __itr->second.end(); ++itr)
        {
            if (itr->second->requestId == id)
                return itr->second;
        }
    }
    return NULL;
}

ContactRequest const* ContactMgr::getContactRequest(uint32 id) const
{
    for (std::map<uint32, std::map<uint32, ContactRequest*>>::const_iterator __itr = _contactRequestsMap.begin();
            __itr != _contactRequestsMap.end(); ++__itr)
    {
        for (std::map<uint32, ContactRequest*>::const_iterator itr = __itr->second.begin();
                itr != __itr->second.end(); ++itr)
        {
            if (itr->second->requestId == id)
                return itr->second;
        }
    }
    return NULL;
}

void ContactMgr::deleteContactRequest(uint32 id)
{
    ContactRequest* req = getContactRequest(id);
    if (req)
        deleteContactRequest(req);
}
