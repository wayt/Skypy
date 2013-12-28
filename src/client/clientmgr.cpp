#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>>
#include "audiomanager.h"
#include "sipPacket.hpp"

ClientMgr::ClientMgr() : _accountId(0), _username(""), _email(""),
    _publicIp(""), _privateIp(""), _activeCallPeerId(0), _requestCallPeerId(0)
{
}

void ClientMgr::makeCall(const QString &destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp)
{
    if (hasCallRequest() || hasActiveCall())
        return;

    QHostAddress host;
    QString destIp = destPublicIp;
    if (_publicIp == destPublicIp)
    {
        host.setAddress(_privateIp);
        destIp = destPrivateIp;
    }
    else
        host.setAddress(_publicIp);

    std::cout << "SEND CALL TO: " << host.toString().toStdString() << std::endl;
    for (quint32 selfPort = AUDIO_PORT; selfPort < AUDIO_PORT + 200; ++selfPort)
        if (sNetworkMgr->setCallHostAddr(host, selfPort))
        {
            SipRequest Rqst("INVITE", sClientMgr->getEmail(), sClientMgr->getAccountId(), host.toString(), selfPort, destEmail, destId, destIp, selfPort);
            sNetworkMgr->tcpSendPacket(Rqst.getPacket());
            setCallRequestPeerId(destId);
            break;
        }
}


void ClientMgr::stopCall(QString const& destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp)
{
    if (!hasActiveCall())
        return;

    QHostAddress host;
    QString destIp = destPublicIp;
    if (_publicIp == destPublicIp)
    {
        host.setAddress(_privateIp);
        destIp = destPrivateIp;
    }
    else
        host.setAddress(_publicIp);

    std::cout << "CANCEL CALL" << std::endl;
    SipRequest Rqst("BYE", sClientMgr->getEmail(), sClientMgr->getAccountId(), host.toString(), 0, destEmail, destId, destIp, 0);
    sNetworkMgr->tcpSendPacket(Rqst.getPacket());

    sClientMgr->setCallRequestPeerId(0);
    sClientMgr->setActiveCallPeerId(0);
    sAudioManager->quit();
    sNetworkMgr->quitCall();
}
