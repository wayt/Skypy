#include "Session.h"
#include "SessionSocket.h"
#include "Skypy.h"

Session::Session(uint32 id, SessionSocket* sock) : _id(id), _socket(sock), _packetQueue(),
    _logout(false)
{}

void Session::logout()
{
    _socket = NULL;
    _logout = true;
    sSkypy->delSession(this);
}

void Session::update(uint32 diff)
{
    if (!_socket || _logout)
        return;

    uint32 size = _packetQueue.size();
    for (uint32 i = 0; i < size && !_packetQueue.empty() && _socket && !_logout; ++i)
    {
        Packet* pkt = _packetQueue.get();
        if (!pkt)
            continue;

        OpcodeMgr::OpcodeDefinition const* opcodehandler = OpcodeMgr::getOpcodeDefinition(pkt->getOpcode());

        if (!opcodehandler)
        {
            std::cerr << "Error: receiv an unknow opcode : " << std::hex << pkt->getOpcode() << std::endl;
            delete pkt;
            continue;
        }

        if (opcodehandler->func)
            (this->*opcodehandler->func)(*pkt);

        delete pkt;
    }
}

void Session::handlePacketInput(Packet& pkt)
{
    Packet* newPacket = new Packet(pkt);
    _packetQueue.add(newPacket);
}
