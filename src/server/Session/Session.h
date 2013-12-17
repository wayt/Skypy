#ifndef SESSION_H_
# define SESSION_H_

#include "Packet.hpp"
#include "LockedQueue.hpp"

class SessionSocket;

class Session
{
public:
    Session(uint32 id, SessionSocket* sock);
    uint32 getId() const { return _id; }

    void logout();
    bool isLogout() const { return _logout; }

    void update(uint32 diff);

    void handlePacketInput(Packet& pkt);

private:
    uint32 _id;
    SessionSocket* _socket;
    LockedQueue<Packet> _packetQueue;
    bool _logout;
};

#endif /* !SESSION_H_ */
