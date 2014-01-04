#ifndef WIDGETCONTACTSLIST_H
#define WIDGETCONTACTSLIST_H

#include <QWidget>
#include "ui_widgetcontactslist.h"
#include <map>
#include "widgetchatwindow.h"
#include "sipPacket.hpp"

class WidgetAddContactWindow;

enum NotificationTypes
{
    NOTIF_CONTACT_REQUEST       = 0,
    NOTIF_NEW_MESSAGE           = 1,
};

class Notification : public QListWidgetItem
{
public:
    Notification(QListWidget* parent, NotificationTypes type, QString text, quint32 id) :
        QListWidgetItem(text, parent),
        _type(type), _id(id), _textInfo()
    {}

    NotificationTypes getNotificationType() const { return _type; }
    quint32 getId() const { return _id; }
    QString const& getTextInfo() const { return _textInfo; }
    void setTextInfo(QString const& text) { _textInfo = text; }
private:
    NotificationTypes _type;
    quint32 _id;
    QString _textInfo;
};

class WidgetContactsList : public QWidget, private Ui::WidgetContactsList
{
    Q_OBJECT
public:
    explicit WidgetContactsList(QWidget *parent = 0);

    void initialize();
    void unload();

    void addContact(ContactInfo* info);
    void loginContact(ContactInfo* info);
    void logoutContact(quint32 id);

    QListWidget* getContactListWidget() { return _contactList; }

    void addMessageFrom(quint32 id, QString const& msg, bool notif = false);
    void handleCallResponse(SipRespond const& resp) { _chatWindow->handleCallResponse(resp); }
    void handleCallRequest(ContactInfo const* info, SipRequest const& req) { _chatWindow->handleCallRequest(info, req); }
    void handleByeResponse(SipRespond const& resp) { _chatWindow->handleByeResponse(resp); }
    void handleByeRequest(ContactInfo const* info, SipRequest const& req) { _chatWindow->handleByeRequest(info, req); }

    WidgetAddContactWindow* getContactWindow() { return _addContactWindow; }
    void addNotification(Notification* notif);
    QListWidget* getNotificationWidget() { return _notificationList; }

    void createChatGroup(quint32 id) { _chatWindow->createChatGroup(id); }
    void chatGroupMemberJoin(quint32 id, WidgetChatTab::PeerInfo* peer) { _chatWindow->chatGroupMemberJoin(id, peer); }
    void addChatGroupMessageFrom(quint32 chatId, quint32 fromId, QString const& msg) { _chatWindow->addChatGroupMessageFrom(chatId, fromId, msg); }
    void chatGroupMemberUpdate(quint32 chatId, WidgetChatTab::PeerInfo const& peer) { _chatWindow->chatGroupMemberUpdate(chatId, peer); }

private:
    WidgetChatWindow* _chatWindow;
    WidgetAddContactWindow* _addContactWindow;
    quint32 _accountId;
    QString _name;
    QString _email;

private slots:
    void handleContactDoubleClick(QListWidgetItem* contact);
    void handleNotificationDoubleClick(QListWidgetItem* item);
    void on__addContactButton_clicked();
    void on__tabWidget_currentChanged(int index);
};

#endif // WIDGETCONTACTSLIST_H
