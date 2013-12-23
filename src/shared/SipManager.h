#ifndef SIPMANAGER_H_
# define SIPMANAGER_H_

#include "Packet.hpp"
#include "Singleton.hpp"
#include "SharedDefines.h"

class Session;

class sSipManager : public Singleton<sSipManager>
{
public:
  sSipManager() {}
  void sendSipResponse(Session *peer, int code, const std::string cmd, const std::string &userName, const std::string &contactName, const std::string &contactAdress, const std::string &userAdress, int port, uint32 peerId) {
    Packet pkt(rMSG_SIP);
    peerId = peerId;
    pkt << code;
    pkt << cmd;
    pkt << userName;
    pkt << contactName;
    pkt << contactAdress;
    pkt << peerId;
    pkt << userAdress;
    pkt << port;
    peer->send(pkt);
  }
  void sendSipResponse(Session *peer, int code, const std::string cmd, const std::string &userName, const std::string &contactName, const std::string &contactAdress, uint32 peerId) {
    Packet pkt(rMSG_SIP);
    peerId = peerId;
    pkt << code;
    pkt << cmd;
    pkt << userName;
    pkt << contactName;
    pkt << contactAdress;
    pkt << peerId;
    peer->send(pkt);
  }
  void forwardSip(Session *peer, Packet &pkt)
  {
    peer->send(pkt);
  }
};

#define sSipManager sSipManager::instance()

#endif /* !SKYPY_H_ */
