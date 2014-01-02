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
    void sendSipResponse(Session *peer, uint32 code, std::string const& cmd, std::string const senderEmail, uint32 senderId, std::string const& senderIp, uint32 senderPort, std::string const& destEmail, uint32 destId, std::string const& destIp, uint32 destPort, uint32 chatId) const
    {
        Packet pkt(rMSG_SIP);
        pkt << uint32(code);
        pkt << cmd;
        pkt << senderEmail;
        pkt << uint32(senderId);
        pkt << senderIp;
        pkt << uint32(senderPort);
        pkt << destEmail;
        pkt << uint32(destId);
        pkt << destIp;
        pkt << uint32(destPort);
        pkt << uint32(chatId);
        peer->send(pkt);
    }

    void forwardSip(Session *peer, Packet &pkt) const
    {
        peer->send(pkt);
    }
};

#define sSipManager sSipManager::instance()

#endif /* !SKYPY_H_ */
