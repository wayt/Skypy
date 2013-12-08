#ifndef ASESSION_H_
# define ASESSION_H_

#include "Packet.hpp"

class SessionSocket;

class ASession
{
public:
    ASession(SessionSocket* sock) : _socket(sock) {}

    void sendPkt(Packet const& pkt);
private:
    SessionSocket* _socket;
};

#endif /* !ASESSION_H_ */
