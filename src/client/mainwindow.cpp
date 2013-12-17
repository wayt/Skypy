#include <QMessageBox>
#include <QRegExpValidator>

#include "mainwindow.h"
#include "networkmgr.h"

#include "packet.hpp"
#include "opcodemgr.h"

#include <iostream>

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent),
    _layF(new QFormLayout),
    _leMdp(new QLineEdit(this)),
    _leMail(new QLineEdit(this)),
    _pbConnection(new QPushButton("Connection", this)),
    _networkMgr(this)
{
    this->setCentralWidget(new QWidget(this));
    this->centralWidget()->setLayout(_layF);
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
    if (_leMdp->text().isEmpty() || _leMail->text().isEmpty())
        return ;

    /*
     * Insert your code here
     */

    _networkMgr.tcpConnect("localhost", 5000);
    QMessageBox::information(this, "Authentification", "Authentification successful");
    _networkMgr.makeCall(((_leMail->text()).toStdString()), "192.168.1.11", "TOTO le blaireau");
}

void MainWindow::handleTcpConnected()
{
    QMessageBox::information(this, "Debug", "CONNECTED !");
}

void MainWindow::handleTcpError(QAbstractSocket::SocketError e)
{
    (void)e;
    QMessageBox::information(this, "Debug", "SOCK ERROR");
}

void MainWindow::handleRequireAuth()
{
    Packet pkt(CMSG_AUTH);
    pkt << _leMail->text();
    pkt << _leMdp->text();
    _networkMgr.tcpSendPacket(pkt);
    std::cout << "AUTH SENDED" << std::endl;
    pkt.dumpHex();
}
