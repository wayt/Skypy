#ifndef WIDGETCHATWINDOW_H
#define WIDGETCHATWINDOW_H

#include <QDialog>
#include "ui_widgetchatwindow.h"
#include "sipPacket.hpp"

class ContactInfo;
class WidgetChatTab;

class WidgetChatWindow : public QDialog, private Ui::WidgetChatWindow
{
    Q_OBJECT
public:
    explicit WidgetChatWindow(QWidget *parent = 0);

    WidgetChatTab* addChatTab(ContactInfo const* info, bool selectIt = true);
    WidgetChatTab* getChatTab(quint32 peerId);

    void addMessageFrom(ContactInfo const* info, QString const& msg, bool notif = false);

    void loginContact(quint32 id);
    void logoutContact(quint32 id);

    void handleCallResponse(SipRespond const& resp);
    void handleCallRequest(ContactInfo const* info, SipRequest const& req);
    void handleByeResponse(SipRespond const& resp);
    void handleByeRequest(ContactInfo const* info, SipRequest const& req);

signals:

public slots:

};

#endif // WIDGETCHATWINDOW_H
