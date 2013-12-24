#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>>

ClientMgr::ClientMgr()
{
}

void ClientMgr::makeCall(const QString &destEmail, quint32 destId)
{
    QHostInfo info = QHostInfo::fromName(QHostInfo::localHostName());
    QList<QHostAddress> ips = info.addresses();
    if (ips.size() > 0)
    {
        for (quint32 selfPort = AUDIO_PORT; selfPort < AUDIO_PORT + 200; ++selfPort)
            if (sNetworkMgr->setCallHostAddr(ips[0], selfPort))
            {
                sNetworkMgr->makeCall(destEmail, destId, selfPort);
                break;
            }
    }

}
