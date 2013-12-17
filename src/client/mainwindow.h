#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include "networkmgr.h"
#include "packet.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = 0);
    virtual ~MainWindow();

    void handleRequireAuth(); // When network layout receive a WELCOME msg

    bool handleAuthResult(Packet& pkt);
    void handleServerConnectionLost(QAbstractSocket::SocketError e, QString const& msg);
private:
    QFormLayout *_layF;
    QLineEdit *_leMdp;
    QLineEdit *_leMail;
    QPushButton *_pbConnection;

    NetworkMgr _networkMgr;

private slots:
    void _pbConnection_clicked();


};

#endif // WIDGET_H
