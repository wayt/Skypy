#include "widgetcontactslist.h"
#include <QtGlobal>
#include <iostream>
#include "widgetchatwindow.h"
#include "widgetaddcontactwindow.h"
#include <QMessageBox>
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "clientmgr.h"
#include "audiomanager.h"
#include <QCursor>

WidgetContactsList::WidgetContactsList(QWidget *parent) :
    QWidget(parent),
    Ui::WidgetContactsList(),
    _chatWindow(new WidgetChatWindow(this)),
    _addContactWindow(new WidgetAddContactWindow(this)),
    _contextMenu(new QMenu(this))
{
    setupUi(this);
    QObject::connect(_contactList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleContactDoubleClick(QListWidgetItem*)));
    QObject::connect(_notificationList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(handleNotificationDoubleClick(QListWidgetItem*)));

    _addContactWindow->setModal(true);

    _contactList->setContextMenuPolicy(Qt::CustomContextMenu);
    _contextMenu->addAction("Remove contact", this, SLOT(_handleContactRemove()));
}

void WidgetContactsList::initialize()
{
    _contactList->clear();
    sClientMgr->clearContacts();
    _notificationList->clear();
    sClientMgr->setAccountInfo(0, "", "");
}

void WidgetContactsList::unload()
{
    _chatWindow->hide();
}

void WidgetContactsList::addContact(ContactInfo *info)
{
    if (sClientMgr->addContact(info))
    {
        std::cout << "ADD CONTACT: " << info->text().toStdString() << std::endl;
        if (info->isOnline())
            _chatWindow->loginContact(info->getId());
        _contactList->addItem(info);
    }
}

void WidgetContactsList::loginContact(ContactInfo* info)
{
    _chatWindow->loginContact(info->getId());
}

void WidgetContactsList::logoutContact(quint32 id)
{
    ContactInfo* info = sClientMgr->findContact(id);
    if (!info)
        return;

    _chatWindow->logoutContact(id);

    if (CallPeer* peer = sClientMgr->getCallPeer(id))
    {
        sClientMgr->removeCallPeer(peer);

        if (sClientMgr->getCallPeerCount() == 0)
        {
            sClientMgr->clearCallPeers();
            sAudioManager->terminate();
            sNetworkMgr->quitCall();
        }
    }
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


void WidgetContactsList::addMessageFrom(quint32 id, QString const& msg, bool notif)
{
    ContactInfo* info = sClientMgr->findContact(id);
    if (!info)
        return;

    std::cout << "MSG FROM: " << info->getEmail().toStdString() << " - " << msg.toStdString() << std::endl;
    _chatWindow->addMessageFrom(info, msg, notif);
}

void WidgetContactsList::on__addContactButton_clicked()
{
    _addContactWindow->show();
    _addContactWindow->onShow();
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
            reply = QMessageBox::question(this, "Contact request", "Accept contact request from " + notif->getTextInfo() + " ?",
                                      QMessageBox::Yes | QMessageBox::No);
            switch (reply)
            {
                case QMessageBox::Yes:
                case QMessageBox::No:
                {
                    Packet data(CMSG_ADD_CONTACT_RESPONSE);
                    data << quint32(notif->getId()); // request id
                    data << quint8(reply == QMessageBox::Yes ? 1 : 0);
                    sNetworkMgr->tcpSendPacket(data);

                    break;
                }
                default:
                    break;
            }
        }
        case NOTIF_NEW_MESSAGE:
        {
            _chatWindow->showTabId(notif->getId(), CHAT_TAB_SINGLE);
            break;
        }
    }

    _notificationList->removeItemWidget(notif);
    delete notif;
}

void WidgetContactsList::addNotification(Notification* notif)
{
    if (_tabWidget->currentWidget() != tab_notifications)
    {
        _tabWidget->setTabText(_tabWidget->indexOf(tab_notifications), "Notifications *");
    }

    _notificationList->addItem(notif);
}

void WidgetContactsList::on__tabWidget_currentChanged(int index)
{
   if (index == _tabWidget->indexOf(tab_notifications))
       _tabWidget->setTabText(index, "Notifications");
}

void WidgetContactsList::on__contactList_customContextMenuRequested(const QPoint &pos)
{
   QPoint spawnPos = QCursor::pos();
   _contextMenu->popup(spawnPos);
}

void WidgetContactsList::_handleContactRemove()
{
    QList<QListWidgetItem*> items = _contactList->selectedItems();
    if (items.size() == 1)
    {
        ContactInfo const* info = dynamic_cast<ContactInfo const*>(items.first());
        if (!info)
            return;

        Packet data(CMSG_REMOVE_CONTACT);
        data << quint32(info->getId());
        sNetworkMgr->tcpSendPacket(data);
        _contactList->removeItemWidget(items.first());
        delete info;
    }
}
