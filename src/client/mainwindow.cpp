#include <QMessageBox>
#include <QRegExpValidator>

#include "mainwindow.h"
#include "networkmgr.h"

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
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
}

MainWindow::~MainWindow()
{
}

void MainWindow::_pbConnection_clicked()
{
    //if (_leMdp->text().isEmpty() || _leMail->text().isEmpty())
    //    return ;

    /*
     * Insert your code here
     */

    sNetworkMgr->tcpConnect("localhost", 8000);

    QMessageBox::information(this, "Authentification", "Authentification successful");
}
