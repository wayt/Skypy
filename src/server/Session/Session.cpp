#include "Session.h"
#include "SessionSocket.h"

Session::Session(SessionSocket* sock) : _socket(sock), _packetQueue()
{}

void Session::handlePacketInput(Packet& pkt)
{
    Packet* newPacket = new Packet(pkt);
    _packetQueue.add(newPacket);
}
