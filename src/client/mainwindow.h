#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include <QMenuBar>
#include "packet.hpp"
#include <QStackedWidget>
#include "widgetlogin.h"
#include "widgetcontactslist.h"
#include "networkmgr.h"
#include "sipPacket.hpp"
#include "widgetconfigwindow.h"

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
    void handleJoinChatGroup(Packet& pkt);
    void handleChatGroupAddMember(Packet& pkt);
    void handleGroupChatText(Packet& pkt);
    void handleChatGroupUpdateMember(Packet& pkt);
    void handleContactList(Packet& pkt);
    void handleChatGroupDelMember(Packet& pkt);

    void handlesipResponse(SipRespond const& resp);
    void handleCallRequest(SipRequest const& request);
    void handleByeRequest(SipRequest const& request);

private slots:
    void _handleLogout();
    void _handleSettings();
private:
    QStackedWidget* _widgets;

    WidgetLogin* _loginForm;
    WidgetContactsList* _contactForm;
    QMenuBar* _menuBar;
    WidgetConfigWindow* _configWindow;

};

#endif // WIDGET_H
