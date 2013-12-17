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

void Session::handleSipPacket(Packet& pkt)
{
  std::string cmd;

  pkt >> cmd;
  std::cout << "SIP PACKET RECEIVED" << std::endl;

  if (cmd == "RINVITE")
    {
      std::string user;
      std::string adress;
      std::string contact;
      std::cout << "An User wanna talk" << std::endl;
      pkt >> user;
      pkt >> adress;
      pkt >> contact;
      std::cout << "Composition : " << std::endl << "CMD =" << cmd << std::endl << "USER =" << user << std::endl << "USER ADRESS =" << adress << std::endl << "CONTACT =" << contact << std::endl;
      Packet rep(SMSG_SIP);
      rep << "r";
      rep << 100;
      cmd.erase(0,1);
      rep << cmd;
      rep << user;
      rep << adress;
      rep << contact;
      _socket->send(rep);
    }
}
