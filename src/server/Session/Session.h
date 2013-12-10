#ifndef SESSION_H_
# define SESSION_H_

#include "Packet.hpp"
#include "LockedQueue.hpp"

class SessionSocket;

class Session
{
public:
    Session(SessionSocket* sock);

    void handlePacketInput(Packet& pkt);

private:
    SessionSocket* _socket;
    LockedQueue<Packet> _packetQueue;
};

#endif /* !SESSION_H_ */
