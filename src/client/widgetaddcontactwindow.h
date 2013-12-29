#ifndef WIDGETADDCONTACTWINDOW_H
#define WIDGETADDCONTACTWINDOW_H

#include <QDialog>
#include "ui_widgetaddcontactwindow.h"
#include "widgetcontactslist.h"

class WidgetAddContactWindow : public QDialog, private Ui::WidgetAddContactWindow
{
    Q_OBJECT

public:
    explicit WidgetAddContactWindow(QWidget *parent = 0);

    QListWidget* getResultListWidget() { return _contactResultList; }
    void addResult(ContactInfo* info);

private slots:
    void on__cancelButton_clicked();

    void on__contactEmailInput_returnPressed();

    void on__contactResultList_itemSelectionChanged();

    void on__valideButton_clicked();

    void on__searchButton_clicked();

private:
};

#endif // WIDGETADDCONTACTWINDOW_H
