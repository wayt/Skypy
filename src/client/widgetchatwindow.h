#ifndef WIDGETCHATWINDOW_H
#define WIDGETCHATWINDOW_H

#include <QDialog>
#include "ui_widgetchatwindow.h"
#include "sipPacket.hpp"
#include "widgetchattab.h"
#include "clientmgr.h"

class WidgetChatWindow : public QDialog, private Ui::WidgetChatWindow
{
    Q_OBJECT
public:
    explicit WidgetChatWindow(QWidget *parent = 0);

    WidgetChatTab* addChatTab(ContactInfo const* info, bool selectIt = true);
    WidgetChatTab* addChatTab(quint32 id, bool selectIt = true);
    WidgetChatTab* getChatTab(quint32 peerId, ChatTabTypes type = CHAT_TAB_SINGLE);

    void addMessageFrom(ContactInfo const* info, QString const& msg, bool notif = false);

    void loginContact(quint32 id);
    void logoutContact(quint32 id);

    void handleCallResponse(SipRespond const& resp);
    void handleCallRequest(ContactInfo const* info, SipRequest const& req);
    void handleByeResponse(SipRespond const& resp);
    void handleByeRequest(ContactInfo const* info, SipRequest const& req);

    void createChatGroup(quint32 id);
    void chatGroupMemberJoin(quint32 id, WidgetChatTab::PeerInfo* peer);
    void addChatGroupMessageFrom(quint32 chatId, quint32 fromId, QString const& msg);
    void chatGroupMemberUpdate(quint32 chatId, WidgetChatTab::PeerInfo const& peer);

signals:

public slots:

};

#endif // WIDGETCHATWINDOW_H
