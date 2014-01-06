#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>
#include "audiomanager.h"
#include "sipPacket.hpp"

ClientMgr::ClientMgr() : _accountId(0), _username(""), _email(""),
    _publicIp(""), _privateIp(""), _callPeer(),
    _contactMap()
{
}

void ClientMgr::makeCall(quint32 chatId, const QString &destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp)
{
    /*
    if (chatId == 0 && hasGroupCall())
        return;

    if (hasCallRequestFrom(destId) || hasActiveCallWith(destId))
        return;
        */


    QHostAddress host;
    QString destIp = destPublicIp;
    if (_publicIp == destPublicIp || true)
    {
        host.setAddress(_privateIp);
        destIp = destPrivateIp;
    }
    else
        host.setAddress(_publicIp);

    std::cout << "SEND CALL TO: " << host.toString().toStdString() << std::endl;
    for (quint32 selfPort = AUDIO_PORT; selfPort < AUDIO_PORT + 200; ++selfPort)
        if (sNetworkMgr->addCallHostAddr(destId, host, selfPort))
        {
            SipRequest Rqst("INVITE", sClientMgr->getEmail(), sClientMgr->getAccountId(), host.toString(), selfPort, destEmail, destId, destIp, selfPort, chatId);
            sNetworkMgr->tcpSendPacket(Rqst.getPacket());
            CallPeer* peer = new CallPeer(chatId, destId, destEmail, destIp, selfPort, false);
            sClientMgr->addCallRequest(peer);
            break;
        }
}


void ClientMgr::stopCall(quint32 chatId, QString const& destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp)
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
    SipRequest Rqst("BYE", sClientMgr->getEmail(), sClientMgr->getAccountId(), host.toString(), 0, destEmail, destId, destIp, chatId);
    sNetworkMgr->tcpSendPacket(Rqst.getPacket());

    sClientMgr->clearCallPeers();
    sAudioManager->terminate();
    sNetworkMgr->quitCall();
}

ContactInfo* ClientMgr::findContact(quint32 id)
{
    QMap<quint32, ContactInfo*>::Iterator itr = _contactMap.find(id);
    if (itr == _contactMap.end())
        return NULL;
    return itr.value();
}

ContactInfo const* ClientMgr::findContact(quint32 id) const
{
    QMap<quint32, ContactInfo*>::ConstIterator itr = _contactMap.find(id);
    if (itr == _contactMap.end())
        return NULL;
    return itr.value();
}

bool ClientMgr::addContact(ContactInfo* info)
{
    if (findContact(info->getId()))
        return false;
    _contactMap[info->getId()] = info;
    return true;
}

void ClientMgr::removeContact(quint32 id)
{
    ContactInfo* info = findContact(id);
    if (!info)
        return;
    _contactMap.remove(id);
    delete info;
}
