#ifndef WIDGETCONTACTSLIST_H
#define WIDGETCONTACTSLIST_H

#include <QWidget>
#include "ui_widgetcontactslist.h"
#include <map>
#include "widgetchatwindow.h"

class WidgetAddContactWindow;

class ContactInfo : public QListWidgetItem
{
public:
    ContactInfo(QListWidget* parent, quint32 id, QString const& name, QString const& email, bool online = true) :
        QListWidgetItem(QString(name + " (" + email + ") - " + (online ? "Online" : "Offline")), parent),
        _id(id), _name(name), _email(email), _online(online)
    {}

    quint32 getId() const { return _id; }
    QString const& getName() const { return _name; }
    QString const& getEmail() const { return _email; }
    bool isOnline() const { return _online; }

private:
    quint32 _id;
    QString _name;
    QString _email;
    bool _online;
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

    void addMessageFrom(quint32 id, QString const& msg);

    WidgetAddContactWindow* getContactWindow() { return _addContactWindow; }
    QListWidget* getNotificationWidget() { return _notificationList; }

private:
    std::map<quint32, ContactInfo*> _contactMap;
    WidgetChatWindow* _chatWindow;
    WidgetAddContactWindow* _addContactWindow;

private slots:
    void handleContactDoubleClick(QListWidgetItem* contact);
    void handleNotificationDoubleClick(QListWidgetItem* item);
    void on__addContactButton_clicked();
};

#endif // WIDGETCONTACTSLIST_H
