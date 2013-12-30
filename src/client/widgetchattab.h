#ifndef WIDGETCHATTAB_H
#define WIDGETCHATTAB_H

#include <QWidget>
#include "ui_widgetchattab.h"
#include "sipPacket.hpp"

class ContactInfo;
class WidgetChatTabAddWindow;

enum ChatTabTypes
{
    CHAT_TAB_SINGLE     = 0,
    CHAT_TAB_MULTI      = 1,
};

class WidgetChatTab : public QWidget, private Ui::WidgetChatTab
{
    Q_OBJECT

public:
    explicit WidgetChatTab(ContactInfo const* info, QWidget *parent = 0);
    WidgetChatTab(quint32 id, QWidget* parent = 0);

    QListWidget* getChatTable() { return _chatTable; }
    //quint32 getPeerId() const { return _peerId; }
    //QString const& getPeerName() const { return _peerName; }
    //QString const& getPeerEmail() const { return _peerEmail; }
    quint32 getTabId() const { return _tabId; }
    void setTabId(quint32 id) { _tabId = id; }

    void loginContact(quint32 id);
    void logoutContact(quint32 id);

    void addMessage(quint32 id, QString const& msg, bool notif);
    void addMessage(QString const& msg);
    void handleCallResponse(SipRespond const& resp);
    void handleCallRequest(SipRequest const& req);
    void handleByeRequest(SipRequest const& req);

    struct PeerInfo
    {
        quint32 peerId;
        QString peerName;
        QString peerEmail;
        QString peerPublicIp;
        QString peerPrivateIp;
        bool online;
    };

    PeerInfo const* getPeerInfo(quint32 id) const;
    ChatTabTypes getTabType() const { return _tabType; }

    void memberJoin(PeerInfo* peer);
    QString getTabName() const;

private slots:
    void on__callButon_clicked();

    void on__inputText_returnPressed();

    void on__sendButton_clicked();

    void on__addButton_clicked();

private:
    quint32 _getOnlinePeerCount() const;
    PeerInfo* _getPeerInfo(quint32 id);
    PeerInfo const* _getFirstPeer() const;

    quint32 _tabId;
    QMap<quint32, PeerInfo*> _peersMap;
    ChatTabTypes _tabType;
    WidgetChatTabAddWindow* _addWindow;

    /*
    quint32 _peerId;
    QString _peerName;
    QString _peerEmail;
    QString _peerPublicIp;
    QString _peerPrivateIp;
    bool _online;
    */
};

#endif // WIDGETCHATTAB_H
