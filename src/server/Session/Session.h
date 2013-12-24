#ifndef SESSION_H_
# define SESSION_H_

#include "Packet.hpp"
#include "LockedQueue.hpp"
#include "ContactMgr.h"

class SessionSocket;

class Session
{
public:
    Session(SessionSocket* sock, std::string const& email);
    uint32 getId() const { return _id; }
    std::string const& getName() const { return _name; }
    std::string const& getEmail() const { return _email; }

    inline std::string const& getRemoteAddess() const;

    void logout();
    bool isLogout() const { return _logout; }
    bool loadFromDb();
    bool saveToDb() const;

    void update(uint32 diff);
    void send(Packet const& pkt);

    void handlePacketInput(Packet& pkt);
    void handleSipPacket(Packet& pkt);
    void handleSipRespond(Packet& pkt);
    void handleChatText(Packet& pkt);
    void handleSearchNewContact(Packet& pkt);
    void handleAddContactRequest(Packet& pkt);
    void handleAddContactResponse(Packet& pkt);

    bool hasFriend(Session const* sess) const { return hasFriend(sess->getId()); }
    bool hasFriend(uint32 id) const;
    void friendLogin(Session* sess);
    void addFriend(ContactInfo* info);
    void broadcastToFriend(Packet const& pkt) const;
    void buildOnlineFriendPacket(Packet& pkt) const;

    void sendContactRequest();

    std::string const& getHostAddress() const;

private:
    void _loadAccountInfo();

    uint32 _id;
    SessionSocket* _socket;
    LockedQueue<Packet> _packetQueue;
    bool _logout;
    std::string _name;
    std::string _email;
};

#endif /* !SESSION_H_ */
