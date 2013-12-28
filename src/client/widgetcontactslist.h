#ifndef WIDGETCONTACTSLIST_H
#define WIDGETCONTACTSLIST_H

#include <QWidget>
#include "ui_widgetcontactslist.h"
#include <map>
#include "widgetchatwindow.h"
#include "sipPacket.hpp"

class WidgetAddContactWindow;

class ContactInfo : public QListWidgetItem
{
public:
    ContactInfo(QListWidget* parent, quint32 id, QString const& name, QString const& email, bool online = true, QString const& ipPublic = "", QString const& ipPrivate = "") :
        QListWidgetItem(QString(name + " (" + email + ") - " + (online ? "Online" : "Offline")), parent),
        _id(id), _name(name), _email(email), _online(online), _publicIp(ipPublic), _privateIp(ipPrivate)
    {}

    quint32 getId() const { return _id; }
    QString const& getName() const { return _name; }
    QString const& getEmail() const { return _email; }
    bool isOnline() const { return _online; }
    QString const& getPublicIp() const { return _publicIp; }
    QString const& getPrivateIp() const { return _privateIp; }

private:
    quint32 _id;
    QString _name;
    QString _email;
    bool _online;
    QString _publicIp;
    QString _privateIp;
};

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

    ContactInfo* findContact(quint32 id);

    QListWidget* getContactListWidget() { return _contactList; }

    void addMessageFrom(quint32 id, QString const& msg, bool notif = false);
    void handleCallResponse(SipRespond const& resp) { _chatWindow->handleCallResponse(resp); }
    void handleCallRequest(ContactInfo const* info, SipRequest const& req) { _chatWindow->handleCallRequest(info, req); }

    WidgetAddContactWindow* getContactWindow() { return _addContactWindow; }
    QListWidget* getNotificationWidget() { return _notificationList; }

private:
    std::map<quint32, ContactInfo*> _contactMap;
    WidgetChatWindow* _chatWindow;
    WidgetAddContactWindow* _addContactWindow;
    quint32 _accountId;
    QString _name;
    QString _email;

private slots:
    void handleContactDoubleClick(QListWidgetItem* contact);
    void handleNotificationDoubleClick(QListWidgetItem* item);
    void on__addContactButton_clicked();
    void on__debugButton_clicked();
};

#endif // WIDGETCONTACTSLIST_H
