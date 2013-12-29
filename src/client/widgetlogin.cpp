#include "widgetlogin.h"
#include "networkmgr.h"

WidgetLogin::WidgetLogin(QWidget *parent) :
    QWidget(parent),
    Ui::WidgetLogin()
{
    setupUi(this);
}

void WidgetLogin::initialize()
{

}

void WidgetLogin::unload()
{

}

void WidgetLogin::on__connectButton_clicked()
{
    if (_emailInput->text().isEmpty() || _passwordInput->text().isEmpty())
        return ;

    _email = _emailInput->text();
    _password = _passwordInput->text();

    /*
     * Insert your code here
     */

    sNetworkMgr->tcpConnect("wayt.me", 5000);
}
