#ifndef CLIENTMGR_H
#define CLIENTMGR_H

#include "singleton.h"
#include <QString>
#include <QtCore>
#include <QtGlobal>

class ClientMgr : public Singleton<ClientMgr>
{
public:
    ClientMgr();

    void setAccountInfo(quint32 id, QString const& name, QString const& email) { _accountId = id; _username = name; _email = email; }
    quint32 getAccountId() const { return _accountId; }
    QString const& getUserName() const { return _username; }
    QString const& getEmail() const { return _email; }
    QString const& getPublicIp() const { return _publicIp; }
    void setPublicIp(QString const& ip) { _publicIp = ip; }
    QString const& getPrivateIp() const { return _privateIp; }
    void setPrivateIp(QString const& ip) { _privateIp = ip; }

    bool hasActiveCall() const { return _activeCallPeerId > 0; }
    quint32 getActiveCallPeerId() const { return _activeCallPeerId; }
    void setActiveCallPeerId(quint32 id) { _activeCallPeerId = id; }

    bool hasCallRequest() const { return _requestCallPeerId > 0; }
    quint32 getCallRequestPeerId() const { return _requestCallPeerId; }
    void setCallRequestPeerId(quint32 id) { _requestCallPeerId = id; }


    void makeCall(QString const& destEmail, quint32 destId, QString const& destPublicIp, QString const& destPrivateIp);

private:
    quint32 _accountId;
    QString _username;
    QString _email;
    QString _publicIp;
    QString _privateIp;

    quint32 _activeCallPeerId;
    quint32 _requestCallPeerId;
};

#define sClientMgr ClientMgr::instance()

#endif // CLIENTMGR_H
