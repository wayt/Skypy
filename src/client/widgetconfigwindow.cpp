#include "widgetconfigwindow.h"

WidgetConfigWindow::WidgetConfigWindow(QWidget *parent) :
    QDialog(parent),
    Ui::WidgetConfigWindow()
{
    setupUi(this);
}

void WidgetConfigWindow::show()
{
    QDialog::show();
}
