#include "Session.h"
#include "SessionSocket.h"
#include "Skypy.h"
#include "SipManager.h"

Session::Session(uint32 id, SessionSocket* sock, std::string const& email) : _id(id), _socket(sock), _packetQueue(),
    _logout(false), _name(email), _email(email)
{
    std::size_t found = _email.find('@');
    if (found != std::string::npos)
        _name = _email.substr(0, found);
}

std::string const& Session::getRemoteAddess() const
{
    return _socket->getRemoteAddress();
}

void Session::logout()
{
    delete _socket;
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

void Session::send(Packet const& pkt)
{
    if (_socket && !_logout)
        _socket->send(pkt);
}

void Session::handleSipPacket(Packet& pkt)
{
  std::string cmd;
  uint32 peerId;
  std::string user;
  std::string adress;
  std::string contact;

  std::cout << "SIP REQUESTPACKET RECEIVED" << std::endl;
  pkt >> cmd;
  pkt >> user >> contact >> adress >> peerId;
  sSipManager->sendSipResponse(this/*100, cmd, user, adress, contact*/);
  Session* peer = sSkypy->findSession(peerId);
  if (!peer)
    {
      std::cout << "Peer not found" << std::endl;
      sSipManager->sendSipResponse(this/*, 404, cmd, user, adress, contact*/);
      return;
    }
  else
    {
      sSipManager->forwardSip(peer, pkt);
      if (cmd == "INVITE") /*On separe le fait de sonner du fait d'attendre une reponse*/
	sSipManager->sendSipResponse(this/*, 180, cmd, user, adress, contact*/);
    }
}

void Session::handleChatText(Packet& pkt)
{
    uint32 peerId;
    std::string msg;
    pkt >> peerId >> msg;

    Session* peer = sSkypy->findSession(peerId);
    if (!peer)
        return;

    Packet data(SMSG_CHAT_TEXT);
    data << uint32(getId());
    data << msg;
    peer->send(data);
}
