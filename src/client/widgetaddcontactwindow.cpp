#include "widgetaddcontactwindow.h"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "clientmgr.h"

WidgetAddContactWindow::WidgetAddContactWindow(QWidget *parent) :
    QDialog(parent),
    Ui::WidgetAddContactWindow()
{
    setupUi(this);
}

void WidgetAddContactWindow::on__cancelButton_clicked()
{
    close();
}

void WidgetAddContactWindow::on__contactEmailInput_returnPressed()
{
    Packet data(CMSG_SEARCH_NEW_CONTACT);
    data << _contactEmailInput->text();
    sNetworkMgr->tcpSendPacket(data);
}

void WidgetAddContactWindow::on__searchButton_clicked()
{
    on__contactEmailInput_returnPressed();
}

void WidgetAddContactWindow::addResult(ContactInfo *info)
{
    _contactResultList->addItem(info);
}

void WidgetAddContactWindow::on__contactResultList_itemSelectionChanged()
{
    QList<QListWidgetItem*> items = _contactResultList->selectedItems();
    _valideButton->setEnabled(items.size() > 0);
}

void WidgetAddContactWindow::on__valideButton_clicked()
{
    QList<QListWidgetItem*> items = _contactResultList->selectedItems();
    if (QListWidgetItem* item = items.first())
        if (ContactInfo* info = dynamic_cast<ContactInfo*>(item))
        {
            Packet data(CMSG_ADD_CONTACT_REQUEST);
            data << info->getEmail();
            sNetworkMgr->tcpSendPacket(data);
            hide();
        }
}
