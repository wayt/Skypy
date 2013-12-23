#ifndef WIDGET_H
#define WIDGET_H

#include <QMainWindow>
#include <QLineEdit>
#include "packet.hpp"
#include <QStackedWidget>
#include "widgetloginform.h"
#include "widgetcontactslist.h"
#include "networkmgr.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QMainWindow *parent = 0);
    virtual ~MainWindow();

    void handleRequireAuth(); // When network layout receive a WELCOME msg

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

    void handlesipResponse(sipRequest const* req, sipRespond const* resp);
    void handleCallRequest(std::string const& userName, std::string const& contactName, std::string const& contactAddress, quint32 peerId);
private:
    QStackedWidget* _widgets;

    WidgetLoginForm* _loginForm;
    WidgetContactsList* _contactForm;

};

#endif // WIDGET_H
