#ifndef SESSION_H_
# define SESSION_H_

#include "Packet.hpp"
#include "LockedQueue.hpp"

class SessionSocket;

class Session
{
public:
    Session(uint32 id, SessionSocket* sock, std::string const& email);
    uint32 getId() const { return _id; }
    std::string const& getName() const { return _name; }
    std::string const& getEmail() const { return _email; }

    inline std::string const& getRemoteAddess() const;

    void logout();
    bool isLogout() const { return _logout; }

    void update(uint32 diff);
    void send(Packet const& pkt);

    void handlePacketInput(Packet& pkt);
    void handleSipPacket(Packet& pkt);
    void handleChatText(Packet& pkt);


private:
    uint32 _id;
    SessionSocket* _socket;
    LockedQueue<Packet> _packetQueue;
    bool _logout;
    std::string _name;
    std::string _email;
};

#endif /* !SESSION_H_ */
