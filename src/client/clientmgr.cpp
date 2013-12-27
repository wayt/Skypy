#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>>

ClientMgr::ClientMgr()
{
}

void ClientMgr::makeCall(const QString &destEmail, quint32 destId)
{
    QHostAddress host("0.0.0.0");
    for (quint32 selfPort = AUDIO_PORT; selfPort < AUDIO_PORT + 2000; ++selfPort)
        if (sNetworkMgr->setCallHostAddr(host, selfPort))
        {
            sNetworkMgr->makeCall(destEmail, destId, sClientMgr->getPublicIp(), selfPort);
            break;
        }
}
