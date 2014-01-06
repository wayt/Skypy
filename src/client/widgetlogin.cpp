#include <QMessageBox>

#include "widgetlogin.h"
#include "networkmgr.h"
#include "clientmgr.h"

WidgetLogin::WidgetLogin(QWidget *parent) :
    QWidget(parent),
    Ui::WidgetLogin(),
    _emailRegex("^[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}$", Qt::CaseInsensitive, QRegExp::RegExp)
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
    if (!_emailRegex.exactMatch(_emailInput->text()))
    {
        QMessageBox::critical(this, "Error", "You'r email address is not valid");
        return ;
    }

    _email = _emailInput->text();
    _password = _passwordInput->text();

    QString addr = sClientMgr->settings().value("serverHost", "wayt.me").toString();
    quint16 port = sClientMgr->settings().value("serverPort", 5000).toUInt();
    sNetworkMgr->tcpConnect(addr, port);
}
