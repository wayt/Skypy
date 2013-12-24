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

    void makeCall(QString const& destEmail, quint32 destId);

private:
    quint32 _accountId;
    QString _username;
    QString _email;
};

#define sClientMgr ClientMgr::instance()

#endif // CLIENTMGR_H
