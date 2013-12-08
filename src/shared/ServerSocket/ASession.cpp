#include "ASession.hpp"
#include "SessionSocket.hpp"

void ASession::sendPkt(Packet const& pkt)
{
    _socket->send(pkt);
}
