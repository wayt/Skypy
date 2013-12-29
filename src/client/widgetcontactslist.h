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
};

class Notification : public QListWidgetItem
{
public:
    Notification(QListWidget* parent, NotificationTypes type, QString text, ContactInfo* sender) :
        QListWidgetItem(text, parent),
        _type(type), _sender(sender)
    {}
    virtual ~Notification()
    {
        delete _sender;
    }

    NotificationTypes getNotificationType() const { return _type; }
    ContactInfo const* getSender() const { return _sender; }
private:
    NotificationTypes _type;
    ContactInfo* _sender;
};

class WidgetContactsList : public QWidget, private Ui::WidgetContactsList
{
    Q_OBJECT
public:
    explicit WidgetContactsList(QWidget *parent = 0);

    void initialize();
    void unload();

    void loginContact(ContactInfo* info);
    void logoutContact(quint32 id);

    QListWidget* getContactListWidget() { return _contactList; }

    void addMessageFrom(quint32 id, QString const& msg, bool notif = false);
    void handleCallResponse(SipRespond const& resp) { _chatWindow->handleCallResponse(resp); }
    void handleCallRequest(ContactInfo const* info, SipRequest const& req) { _chatWindow->handleCallRequest(info, req); }
    void handleByeResponse(SipRespond const& resp) { _chatWindow->handleByeResponse(resp); }
    void handleByeRequest(ContactInfo const* info, SipRequest const& req) { _chatWindow->handleByeRequest(info, req); }

    WidgetAddContactWindow* getContactWindow() { return _addContactWindow; }
    QListWidget* getNotificationWidget() { return _notificationList; }

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
};

#endif // WIDGETCONTACTSLIST_H
