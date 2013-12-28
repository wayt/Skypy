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

WidgetChatTab* WidgetChatWindow::getChatTab(QString const& tabName)
{
    int size = _chatTab->count();
    for (int i = 0; i < size; ++i)
        if (_chatTab->tabText(i) == tabName)
            return dynamic_cast<WidgetChatTab*>(_chatTab->widget(i));
    return NULL;
}

WidgetChatTab* WidgetChatWindow::addChatTab(ContactInfo* info, bool selectIt)
{
    WidgetChatTab* tab = getChatTab(info->getName());
    if (!tab)
    {
        tab = new WidgetChatTab(info, this);
        _chatTab->addTab(tab, info->getName());
    }
    if (selectIt)
        _chatTab->setCurrentWidget(tab);
    return tab;
}


void WidgetChatWindow::addMessageFrom(ContactInfo* info, QString const& msg, bool notif)
{
    WidgetChatTab* tab = getChatTab(info->getName());
    if (!tab)
        tab = addChatTab(info, false);
    std::cout << "ADD TAB MSG FROM: " << info->getEmail().toStdString() << " - " << msg.toStdString() << std::endl;
    QString item = msg;
    if (!notif)
       item = info->getName() + ": " + msg;
    tab->getChatTable()->addItem(item);
    tab->getChatTable()->scrollToBottom();
}
