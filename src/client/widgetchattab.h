#ifndef WIDGETCHATTAB_H
#define WIDGETCHATTAB_H

#include <QWidget>
#include "ui_widgetchattab.h"
#include "sipPacket.hpp"

class ContactInfo;

class WidgetChatTab : public QWidget, private Ui::WidgetChatTab
{
    Q_OBJECT

public:
    explicit WidgetChatTab(ContactInfo const* info, QWidget *parent = 0);

    QListWidget* getChatTable() { return _chatTable; }
    quint32 getPeerId() const { return _peerId; }
    QString const& getPeerName() const { return _peerName; }
    QString const& getPeerEmail() const { return _peerEmail; }

    void loginContact();
    void logoutContact();

    void addMessage(QString const& msg, bool notif);
    void handleCallResponse(SipRespond const& resp);
    void handleCallRequest(SipRequest const& req);
    void handleByeRequest(SipRequest const& req);

private slots:
    void on__callButon_clicked();

    void on__inputText_returnPressed();

    void on__sendButton_clicked();

private:
    quint32 _peerId;
    QString _peerName;
    QString _peerEmail;
    QString _peerPublicIp;
    QString _peerPrivateIp;
    bool _online;
};

#endif // WIDGETCHATTAB_H
