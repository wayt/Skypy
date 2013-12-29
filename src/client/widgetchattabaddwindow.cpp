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
