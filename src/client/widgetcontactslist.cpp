#include "widgetcontactslist.h"

WidgetContactsList::WidgetContactsList(QWidget *parent) :
    QWidget(parent),
    Ui::WidgetContactsList(),
    _contactMap()
{
    setupUi(this);
}

void WidgetContactsList::initialize()
{
    _contactMap.clear();
    _contactList->clear();
}

void WidgetContactsList::loginContact(ContactInfo* info)
{
    _contactList->addItem(info);
    _contactMap[info->getId()] = info;
}

void WidgetContactsList::logoutContact(quint32 id)
{
    std::map<quint32, ContactInfo*>::const_iterator itr = _contactMap.find(id);
    if (itr == _contactMap.end())
        return;

    ContactInfo* info = itr->second;
    _contactList->removeItemWidget(info);
    _contactMap.erase(id);
    delete info;
}
