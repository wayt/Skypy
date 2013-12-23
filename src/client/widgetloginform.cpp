#include "widgetloginform.h"
#include <QRegExpValidator>
#include "networkmgr.h"

WidgetLoginForm::WidgetLoginForm(QWidget *parent) :
    QWidget(parent),
    _layF(new QFormLayout(this)),
    _leMdp(new QLineEdit(this)),
    _leMail(new QLineEdit(this)),
    _pbConnection(new QPushButton("Connection", this))
{
    _layF->addRow("Mail address", _leMail);
    _layF->addRow("Password", _leMdp);
    _layF->addWidget(_pbConnection);

    _leMdp->setEchoMode(QLineEdit::Password);
    _leMail->setValidator(new QRegExpValidator(QRegExp("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+.[A-Za-z]{2,6}$", Qt::CaseSensitive, QRegExp::RegExp2), this));

    QObject::connect(_pbConnection, SIGNAL(clicked()), this, SLOT(_pbConnection_clicked()));

    _leMail->setText("test@gmail.com");
    _leMdp->setText("titi");
}

void WidgetLoginForm::initialize()
{

}

void WidgetLoginForm::unload()
{

}

void WidgetLoginForm::_pbConnection_clicked()
{
    if (_leMdp->text().isEmpty() || _leMail->text().isEmpty())
        return ;

    /*
     * Insert your code here
     */

    sNetworkMgr->tcpConnect("localhost", 5000);
}

