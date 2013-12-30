#include "widgetchattabaddwindow.h"
#include <iostream>
#include "networkmgr.h"
#include "packet.hpp"
#include "opcodemgr.h"
#include "widgetchattab.h"
#include "clientmgr.h"

WidgetChatTabAddWindow::WidgetChatTabAddWindow(WidgetChatTab *parent) :
    QDialog(parent),
    Ui::WidgetChatTabAddWindow(),
    _tabId(parent->getTabId()),
    _chatTab(parent)
{
    setupUi(this);
    setModal(true);
}

void WidgetChatTabAddWindow::onShow()
{
    _contactList->clear();
    QMap<quint32, ContactInfo*> const& contacts = sClientMgr->getContactMap();
    for (QMap<quint32, ContactInfo*>::ConstIterator itr = contacts.begin();
         itr != contacts.end(); ++itr)
    {
       if (!_chatTab->getPeerInfo(itr.value()->getId()))
       {
           ContactInfo* peer = new ContactInfo(_contactList, itr.value());
           _contactList->addItem(peer);
       }
    }
}

void WidgetChatTabAddWindow::on__cancelButton_clicked()
{
   close();
}

void WidgetChatTabAddWindow::on__contactList_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = _contactList->selectedItems();
    _okButton->setEnabled(items.size() > 0);
}

void WidgetChatTabAddWindow::on__okButton_clicked()
{
    QList<QListWidgetItem*> items = _contactList->selectedItems();
    if (items.size() == 0)
        return;

    Packet data(CMSG_CHAT_GROUP_ADD_MEMBERS);
    data << quint32(_chatTab->getTabId()); // Currente tab id
    data << quint32(items.size()); // Add member count
    for (QList<QListWidgetItem*>::ConstIterator itr = items.begin();
         itr != items.end(); ++itr)
        data << quint32(dynamic_cast<ContactInfo*>(*itr)->getId());
    sNetworkMgr->tcpSendPacket(data);

    close();
}
