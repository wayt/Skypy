#include "widgetcontactslist.h"
#include <QtGlobal>
#include <iostream>
#include "widgetchatwindow.h"
#include "widgetaddcontactwindow.h"
#include <QMessageBox>
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"

WidgetContactsList::WidgetContactsList(QWidget *parent) :
    QWidget(parent),
    Ui::WidgetContactsList(),
    _contactMap(),
    _chatWindow(new WidgetChatWindow(this)),
    _addContactWindow(new WidgetAddContactWindow(this))
{
    setupUi(this);
    QObject::connect(_contactList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleContactDoubleClick(QListWidgetItem*)));
    QObject::connect(_notificationList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleNotificationDoubleClick(QListWidgetItem*)));

    _addContactWindow->setModal(true);
}

void WidgetContactsList::initialize()
{
    _contactMap.clear();
    _contactList->clear();
    _notificationList->clear();
}

void WidgetContactsList::unload()
{
    _chatWindow->hide();
}

void WidgetContactsList::loginContact(ContactInfo* info)
{
    _contactList->addItem(info);
    _contactMap[info->getId()] = info;
}

void WidgetContactsList::logoutContact(quint32 id)
{
    ContactInfo* info = findContact(id);
    if (!info)
        return;

    _contactList->removeItemWidget(info);
    _contactMap.erase(id);
    delete info;
}

void WidgetContactsList::handleContactDoubleClick(QListWidgetItem* item)
{
    ContactInfo* contact = dynamic_cast<ContactInfo*>(item);
    std::cout << "CLICKED ON " << contact->getEmail().toStdString() << std::endl;

    if (contact)
    {
        _chatWindow->addChatTab(contact, true);
        _chatWindow->show();
    }
}


ContactInfo* WidgetContactsList::findContact(quint32 id)
{
    for (std::map<quint32, ContactInfo*>::const_iterator itr = _contactMap.begin();
         itr != _contactMap.end(); ++itr)
        std::cout << "ID: " << itr->first << " - " << itr->second->getEmail().toStdString() << std::endl;
    std::map<quint32, ContactInfo*>::const_iterator itr = _contactMap.find(id);
    if (itr == _contactMap.end())
        return NULL;
    return itr->second;
}


void WidgetContactsList::addMessageFrom(quint32 id, QString const& msg, bool notif)
{
    ContactInfo* info = findContact(id);
    if (!info)
        return;

    std::cout << "MSG FROM: " << info->getEmail().toStdString() << " - " << msg.toStdString() << std::endl;
    _chatWindow->addMessageFrom(info, msg, notif);
}

void WidgetContactsList::on__addContactButton_clicked()
{
    _addContactWindow->show();
}

void WidgetContactsList::handleNotificationDoubleClick(QListWidgetItem* item)
{
    Notification* notif = dynamic_cast<Notification*>(item);
    if (!notif)
    {
        _notificationList->removeItemWidget(item);
        return;
    }

    switch (notif->getNotificationType())
    {
        case NOTIF_CONTACT_REQUEST:
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "Contact request", "Accept contact request from " + notif->getSender()->text() + " ?",
                                      QMessageBox::Yes | QMessageBox::No);
            switch (reply)
            {
                case QMessageBox::Yes:
                case QMessageBox::No:
                {
                    Packet data(CMSG_ADD_CONTACT_RESPONSE);
                    data << quint32(notif->getSender()->getId()); // request id
                    data << quint8(reply == QMessageBox::Yes ? 1 : 0);
                    std::cout << (reply == QMessageBox::Yes ? "Accept" : "Refuse") << " contact request " << notif->getSender()->getId() << std::endl;
                    sNetworkMgr->tcpSendPacket(data);

                    _notificationList->removeItemWidget(notif);
                    delete notif;
                    break;
                }
            }
        }
    }

}
