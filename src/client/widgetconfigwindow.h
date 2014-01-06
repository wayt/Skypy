#ifndef WIDGETCONFIGWINDOW_H
#define WIDGETCONFIGWINDOW_H

#include <QDialog>
#include "ui_widgetconfigwindow.h"


class WidgetConfigWindow : public QDialog, private Ui::WidgetConfigWindow
{
    Q_OBJECT

public:
    explicit WidgetConfigWindow(QWidget *parent = 0);

    void show();
private slots:
    void on_buttonBox_accepted();

private:
};

#endif // WIDGETCONFIGWINDOW_H
