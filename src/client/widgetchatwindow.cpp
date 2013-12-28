#include "widgetchatwindow.h"
#include "widgetcontactslist.h"
#include "widgetchattab.h"
#include <iostream>

WidgetChatWindow::WidgetChatWindow(QWidget *parent) :
    QDialog(parent),
    Ui::WidgetChatWindow()
{
    setupUi(this);
}

WidgetChatTab* WidgetChatWindow::getChatTab(QString const& email)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (WidgetChatTab* tab = dynamic_cast<WidgetChatTab*>(_chatTab->widget(i)))
            if (tab->getPeerEmail() == email)
                return tab;
    return NULL;
}

WidgetChatTab* WidgetChatWindow::getChatTab(quint32 id)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (WidgetChatTab* tab = dynamic_cast<WidgetChatTab*>(_chatTab->widget(i)))
            if (tab->getPeerId() == id)
                return tab;
    return NULL;
}


WidgetChatTab* WidgetChatWindow::addChatTab(ContactInfo const* info, bool selectIt)
{
    WidgetChatTab* tab = getChatTab(info->getEmail());
    if (!tab)
    {
        tab = new WidgetChatTab(info, this);
        _chatTab->addTab(tab, info->getName());
    }
    if (selectIt)
        _chatTab->setCurrentWidget(tab);
    return tab;
}


void WidgetChatWindow::addMessageFrom(ContactInfo const* info, QString const& msg, bool notif)
{
    WidgetChatTab* tab = getChatTab(info->getEmail());
    if (!tab)
        tab = addChatTab(info, false);
    tab->addMessage(msg, notif);
}

void WidgetChatWindow::loginContact(quint32 id)
{
    if (WidgetChatTab* tab = getChatTab(id))
        tab->loginContact();
}

void WidgetChatWindow::logoutContact(quint32 id)
{
    if (WidgetChatTab* tab = getChatTab(id))
        tab->logoutContact();
}

void WidgetChatWindow::handleCallResponse(SipRespond const& resp)
{
    if (WidgetChatTab* tab = getChatTab(resp.getDestId()))
        tab->handleCallResponse(resp);
}

void WidgetChatWindow::handleCallRequest(ContactInfo const* info, SipRequest const& req)
{
    WidgetChatTab* tab = getChatTab(info->getId());
    if (!tab)
        tab = addChatTab(info, false);
    tab->handleCallRequest(req);
}
