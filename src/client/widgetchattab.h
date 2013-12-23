#ifndef WIDGETCHATTAB_H
#define WIDGETCHATTAB_H

#include <QWidget>
#include "ui_widgetchattab.h"

class ContactInfo;

class WidgetChatTab : public QWidget, private Ui::WidgetChatTab
{
    Q_OBJECT

public:
    explicit WidgetChatTab(ContactInfo* info, QWidget *parent = 0);

    QListWidget* getChatTable() { return _chatTable; }

private slots:
    void on__callButon_clicked();

    void on__inputText_returnPressed();

    void on__sendButton_clicked();

private:
    quint32 _peerId;
    QString _peerName;
    QString _peerEmail;
};

#endif // WIDGETCHATTAB_H
