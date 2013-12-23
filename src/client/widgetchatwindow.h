#ifndef WIDGETCHATWINDOW_H
#define WIDGETCHATWINDOW_H

#include <QDialog>
#include "ui_widgetchatwindow.h"

class ContactInfo;
class WidgetChatTab;

class WidgetChatWindow : public QDialog, private Ui::WidgetChatWindow
{
    Q_OBJECT
public:
    explicit WidgetChatWindow(QWidget *parent = 0);

    WidgetChatTab* addChatTab(ContactInfo* info, bool selectIt = true);
    WidgetChatTab* getChatTab(QString const& tabName);

    void addMessageFrom(ContactInfo* info, QString const& msg, bool notif = false);

signals:

public slots:

};

#endif // WIDGETCHATWINDOW_H
