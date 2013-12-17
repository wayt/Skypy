#ifndef WIDGETCONTACTSLIST_H
#define WIDGETCONTACTSLIST_H

#include <QWidget>
#include "ui_widgetcontactslist.h"
#include <map>

class WidgetContactsList : public QWidget, private Ui::WidgetContactsList
{
    Q_OBJECT
public:
    explicit WidgetContactsList(QWidget *parent = 0);

    void initialize();

    class ContactInfo : public QListWidgetItem
    {
    public:
        ContactInfo(QListWidget* parent, quint32 id, QString const& name, QString const& email) :
            QListWidgetItem(QString(name + " (" + email + ")"), parent),
            _id(id), _name(name), _email(email)
        {}

        quint32 getId() const { return _id; }
        QString const& getName() const { return _name; }
        QString const& getEmail() const { return _email; }

    private:
        quint32 _id;
        QString _name;
        QString _email;
    };

    void loginContact(ContactInfo* info);
    void logoutContact(quint32 id);

    QListWidget* getContactListWidget() { return _contactList; }

private:
    std::map<quint32, ContactInfo*> _contactMap;
};

#endif // WIDGETCONTACTSLIST_H
