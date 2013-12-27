#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>>

ClientMgr::ClientMgr()
{
}

void ClientMgr::makeCall(const QString &destEmail, quint32 destId)
{
    QHostAddress host(_publicIp);
    for (quint32 selfPort = AUDIO_PORT; selfPort < AUDIO_PORT + 200; ++selfPort)
        if (sNetworkMgr->setCallHostAddr(host, selfPort))
        {
            sNetworkMgr->makeCall(destEmail, destId, host.toString(), selfPort);
            break;
        }
}
