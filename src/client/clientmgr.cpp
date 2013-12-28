#include "clientmgr.h"
#include <QHostInfo>
#include "networkmgr.h"
#include <iostream>>

ClientMgr::ClientMgr()
{
}

void ClientMgr::makeCall(const QString &destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp)
{
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
            sNetworkMgr->makeCall(host.toString(), selfPort, destEmail, destId, destIp, selfPort);
            break;
        }
}
