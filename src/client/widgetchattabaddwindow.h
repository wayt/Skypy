#ifndef WIDGETCHATTABADDWINDOW_H
#define WIDGETCHATTABADDWINDOW_H

#include <QDialog>
#include "ui_widgetchattabaddwindow.h"

class WidgetChatTab;

class WidgetChatTabAddWindow : public QDialog, private Ui::WidgetChatTabAddWindow
{
    Q_OBJECT

public:
    explicit WidgetChatTabAddWindow(WidgetChatTab* parent);

    void setTabId(quint32 id) { _tabId = id; }
    void onShow();
private slots:
    void on__cancelButton_clicked();

    void on__contactList_itemSelectionChanged();

    void on__okButton_clicked();

private:
    quint32 _tabId;
    WidgetChatTab* _chatTab;
};

#endif // WIDGETCHATTABADDWINDOW_H
