#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include "packet.hpp"
#include <QStackedWidget>
#include "widgetloginform.h"
#include "widgetcontactslist.h"
#include "networkmgr.h"
#include "sipPacket.hpp"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = 0);
    virtual ~MainWindow();

    void handleRequireAuth(QString const& localAddr); // When network layout receive a WELCOME msg

    bool handleAuthResult(Packet& pkt);
    void handleServerConnectionLost(QAbstractSocket::SocketError e, QString const& msg);

    // Network handlers
    void handleSipRep(Packet &);
    void handleSipRequest(Packet &);
    void handleContactLogin(Packet& pkt);
    void handleContactLogout(Packet& pkt);
    void handleChatText(Packet& pkt);
    void handleSearchContactResult(Packet& pkt);
    void handleAddContactRequest(Packet& pkt);
    void handleAccountInfo(Packet& pkt);

    void handlesipResponse(SipRequest const* req, SipRespond const* resp);
    void handleCallRequest(SipRequest const& request);
private:
    QStackedWidget* _widgets;

    WidgetLoginForm* _loginForm;
    WidgetContactsList* _contactForm;

};

#endif // WIDGET_H
