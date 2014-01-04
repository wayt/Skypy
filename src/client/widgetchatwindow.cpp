#include "widgetchatwindow.h"
#include "widgetcontactslist.h"
#include "widgetchattab.h"
#include <iostream>
#include "clientmgr.h"
#include "audiomanager.h"
#include "networkmgr.h"

WidgetChatWindow::WidgetChatWindow(QWidget *parent) :
    QDialog(parent),
    Ui::WidgetChatWindow()
{
    setupUi(this);
}

WidgetChatTab* WidgetChatWindow::getChatTab(quint32 id, ChatTabTypes type)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (WidgetChatTab* tab = dynamic_cast<WidgetChatTab*>(_chatTab->widget(i)))
            if (tab->getTabId() == id && tab->getTabType() == type)
                return tab;
    return NULL;
}


WidgetChatTab* WidgetChatWindow::addChatTab(ContactInfo const* info, bool selectIt)
{
    WidgetChatTab* tab = getChatTab(info->getId());
    if (!tab)
    {
        tab = new WidgetChatTab(info, this);
        _chatTab->addTab(tab, info->getName());

     }


    if (selectIt)
    {
        _chatTab->setCurrentWidget(tab);
        if (this->isHidden())
            this->show();
        if (!hasFocus())
            setFocus();
    }
    return tab;
}

WidgetChatTab* WidgetChatWindow::addChatTab(quint32 id, bool selectIt)
{
    WidgetChatTab* tab = getChatTab(id, CHAT_TAB_MULTI);
    if (!tab)
    {
        tab = new WidgetChatTab(id, this);
        _chatTab->addTab(tab, "");
    }
    if (selectIt)
    {
        _chatTab->setCurrentWidget(tab);
        if (isHidden())
            show();
        if (!hasFocus())
            setFocus();
    }

    return tab;
}


void WidgetChatWindow::addMessageFrom(ContactInfo const* info, QString const& msg, bool notif)
{
    WidgetChatTab* tab = getChatTab(info->getId());
    if ((!tab && !hasFocus()) || isHidden())
    {
        if (WidgetContactsList* parent = dynamic_cast<WidgetContactsList*>(this->parentWidget()))
        {
            bool present = false;
            for (int i = 0; i < parent->getNotificationWidget()->count(); ++i)
                if (Notification* notif = dynamic_cast<Notification*>(parent->getNotificationWidget()->item(i)))
                    if (notif->getId() == info->getId() && notif->getNotificationType() == NOTIF_NEW_MESSAGE)
                    {
                        present = true;
                        break;
                    }

            if (!present)
            {
                Notification* notif = new Notification(parent->getNotificationWidget(), NOTIF_NEW_MESSAGE, "New message from " + info->getName(), info->getId());
                parent->addNotification(notif);
            }
        }
    }
    if (!tab)
        tab = addChatTab(info, false);
    tab->addMessage(info->getId(), msg, notif);
}

void WidgetChatWindow::loginContact(quint32 id)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (WidgetChatTab* tab = dynamic_cast<WidgetChatTab*>(_chatTab->widget(i)))
            if (tab->getTabType() == CHAT_TAB_SINGLE)
                tab->loginContact(id);
}

void WidgetChatWindow::logoutContact(quint32 id)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (WidgetChatTab* tab = dynamic_cast<WidgetChatTab*>(_chatTab->widget(i)))
            if (tab->getTabType() == CHAT_TAB_SINGLE)
                tab->logoutContact(id);

    if (sClientMgr->hasCallRequestFrom(id) ||
            sClientMgr->hasActiveCallWith(id))
    {
        sClientMgr->clearCallPeers();
        sAudioManager->quit();
        sNetworkMgr->quitCall();
    }
}

void WidgetChatWindow::handleCallResponse(SipRespond const& resp)
{
    WidgetChatTab* tab = NULL;
    if (resp.getChatId() > 0)
        tab = getChatTab(resp.getChatId(), CHAT_TAB_MULTI);
    else
        tab = getChatTab(resp.getDestId());

    if (tab)
        tab->handleCallResponse(resp);
}

void WidgetChatWindow::handleCallRequest(ContactInfo const* info, SipRequest const& req)
{
    WidgetChatTab* tab = NULL;
    if (req.getChatId() > 0)
        tab = getChatTab(req.getChatId(), CHAT_TAB_MULTI);
    else
    {
        tab = getChatTab(req.getDestId());
        if (!tab)
            tab = addChatTab(info, false);
    }

    if (tab)
        tab->handleCallRequest(req);
}

void WidgetChatWindow::handleByeResponse(SipRespond const& resp)
{
    Q_UNUSED(resp);
}

void WidgetChatWindow::handleByeRequest(ContactInfo const* info, SipRequest const& req)
{
    if (CallPeer* peer = sClientMgr->getCallPeer(req.getSenderId()))
    {
        sClientMgr->removeCallPeer(peer);
        if (!sClientMgr->hasActiveCall())
        {
            sAudioManager->terminate();
            sNetworkMgr->quitCall();
        }
    }

    WidgetChatTab* tab = getChatTab(info->getId());
    if (!tab)
        tab = addChatTab(info, false);
    tab->handleByeRequest(req);


    SipRespond Rep(200, req);
    sNetworkMgr->tcpSendPacket(Rep.getPacket());
}

void WidgetChatWindow::createChatGroup(quint32 id)
{
    addChatTab(id, false);
}

void WidgetChatWindow::chatGroupMemberJoin(quint32 id, WidgetChatTab::PeerInfo* peer)
{
    WidgetChatTab* tab = getChatTab(id, CHAT_TAB_MULTI);
    if (!tab)
        tab = addChatTab(id, false);
    tab->memberJoin(peer);
    int index = _chatTab->indexOf(tab);
    if (index >= 0)
        _chatTab->setTabText(index, tab->getTabName());
}

void WidgetChatWindow::addChatGroupMessageFrom(quint32 chatId, quint32 fromId, QString const& msg)
{
    WidgetChatTab* tab = getChatTab(chatId, CHAT_TAB_MULTI);
    if (!tab)
        tab = addChatTab(chatId, false);
    tab->addMessage(fromId, msg, false);

}

void WidgetChatWindow::chatGroupMemberUpdate(quint32 chatId, WidgetChatTab::PeerInfo const& peer)
{
    WidgetChatTab* tab = getChatTab(chatId, CHAT_TAB_MULTI);
    if (!tab)
        tab = addChatTab(chatId, false);
    tab->updateMember(peer);
}

void WidgetChatWindow::showTabId(quint32 id, ChatTabTypes type)
{
    WidgetChatTab* tab = getChatTab(id, type);
    if (!tab)
        return;
    _chatTab->setCurrentWidget(tab);
    if (isHidden())
        show();
    if (!hasFocus())
        this->setFocus();

}
