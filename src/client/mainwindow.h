#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include <QFormLayout>
#include <QPushButton>
#include "networkmgr.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = 0);
    virtual ~MainWindow();

    void handleRequireAuth(); // When network layout receive a WELCOME msg

private:
    QFormLayout *_layF;
    QLineEdit *_leMdp;
    QLineEdit *_leMail;
    QPushButton *_pbConnection;

    NetworkMgr _networkMgr;

private slots:
    void _pbConnection_clicked();

public slots:
    void handleTcpConnected();
    void handleTcpError(QAbstractSocket::SocketError e);

};

#endif // WIDGET_H
