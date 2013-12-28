#include "widgetchattab.h"
#include "widgetcontactslist.h"
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "sipPacket.hpp"
#include "audiomanager.h"
#include "clientmgr.h"
#include <QMessageBox>

WidgetChatTab::WidgetChatTab(ContactInfo const* info, QWidget *parent) :
    QWidget(parent),
    Ui::WidgetChatTab(),
    _peerId(info->getId()), _peerName(info->getName()), _peerEmail(info->getEmail()),
    _peerPublicIp(info->getPublicIp()), _peerPrivateIp(info->getPrivateIp()),
    _online(true)
{
    setupUi(this);
}

void WidgetChatTab::on__callButon_clicked()
{
    if (!_online)
        return;
    std::cout << "CLICKED CALL BUTTON" << std::endl;
    if (sClientMgr->getActiveCallPeerId() == _peerId || sClientMgr->getCallRequestPeerId() == _peerId)
    {
        sClientMgr->stopCall(_peerEmail, _peerId, _peerPublicIp, _peerPrivateIp);
        _callButon->setText("Call");
    }
    else
        sClientMgr->makeCall(_peerEmail, _peerId, _peerPublicIp, _peerPrivateIp);
}

void WidgetChatTab::on__inputText_returnPressed()
{
    QString text = _inputText->text().trimmed();
    if (text.size() == 0 || !_online)
        return;

    Packet pkt(CMSG_CHAT_TEXT);
    pkt << quint32(_peerId);
    pkt << text;
    sNetworkMgr->tcpSendPacket(pkt);
    _inputText->setText("");
    _chatTable->addItem(sClientMgr->getUserName() + ": " + text);
    _chatTable->scrollToBottom();
}

void WidgetChatTab::on__sendButton_clicked()
{
    on__inputText_returnPressed();
}

void WidgetChatTab::loginContact()
{
    _online = true;
    _callButon->setEnabled(true);
    _sendButton->setEnabled(true);

    addMessage(_peerName + " logged in !", true);
}

void WidgetChatTab::logoutContact()
{
    _online = false;
    _callButon->setEnabled(false);
    _callButon->setText("Call");
    _sendButton->setEnabled(false);

    addMessage(_peerName + " logged out ...", true);
}

void WidgetChatTab::addMessage(QString const& msg, bool notif)
{
    QString item = msg;
    if (!notif)
       item = _peerName + ": " + msg;
    _chatTable->addItem(item);
    _chatTable->scrollToBottom();
}

void WidgetChatTab::handleCallResponse(SipRespond const& resp)
{
    switch (resp.getCode())
    {
    case 100: // Forward de l'appel
        addMessage("Send call request ...", true);
        _callButon->setText("Stop");
        break;
    case 404: // Contact non connecte
        addMessage(resp.getDestEmail() + " isn't online", true);
        sClientMgr->setCallRequestPeerId(0);
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 180: // Ca sonne
        addMessage("Ringing ...", true);
        break;
    case 200: // Ca a decroche
        addMessage("Call accepted", true);
        sClientMgr->setCallRequestPeerId(0);
        if (sAudioManager->start())
        {
            std::cout << "RECEIV PEER ADDR: " << resp.getDestIp().toStdString() << std::endl;
            sNetworkMgr->setCallPeerAddr(QHostAddress(resp.getDestIp()), resp.getDestPort());
            sNetworkMgr->runCall();
            sClientMgr->setActiveCallPeerId(resp.getDestId());
        }
        else
            std::cout << "FAIL TO START AUDIO" << std::endl;
        break;
    case 603: // Refuse
        addMessage("Call refused", true);
        sClientMgr->setCallRequestPeerId(0);
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 604: // Occuped
        addMessage("Occuped", true);
        sClientMgr->setCallRequestPeerId(0);
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 605: // Peer fail to open network
        addMessage("Peer's network issue", true);
        sClientMgr->setCallRequestPeerId(0);
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 606: // Peer fail to start audio
        addMessage("Peer's audio issue", true);
        sClientMgr->setCallRequestPeerId(0);
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    }
}

void WidgetChatTab::handleCallRequest(SipRequest const& request)
{
    addMessage("Incomming call...", true);
    sClientMgr->setCallRequestPeerId(request.getSenderId());
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Incomming call", "Accept call from " + request.getSenderEmail() + " ?",
                              QMessageBox::Yes | QMessageBox::No);
    switch (reply)
    {
        case QMessageBox::Yes:
        case QMessageBox::No:
        {
            QHostAddress host(request.getDestIp());
            if (reply == QMessageBox::Yes)
            {
                if (sNetworkMgr->setCallHostAddr(host, request.getDestPort()))
                {
                    if (sAudioManager->start())
                    {
                        std::cout << "SET PEER ADDR: " << request.getSenderIp().toStdString() << std::endl;
                        sNetworkMgr->setCallPeerAddr(QHostAddress(request.getSenderIp()), request.getSenderPort());
                        sNetworkMgr->runCall();

                        std::cout << "CALL ACCEPTED, LISTEN ON " << request.getDestIp().toStdString() << ":" << request.getDestPort() << std::endl;
                        SipRespond Rep(200, request);
                        sNetworkMgr->tcpSendPacket(Rep.getPacket());
                        sClientMgr->setCallRequestPeerId(0);
                        sClientMgr->setActiveCallPeerId(request.getSenderId());
                        _callButon->setText("Stop");
                        return;
                    }
                    else // Should send error
                    {
                        std::cout << "FAIL TO START AUDIO" << std::endl;

                        SipRespond Rep(606, request);
                        sNetworkMgr->tcpSendPacket(Rep.getPacket());
                        sClientMgr->setCallRequestPeerId(0);
                    }
                }
                else
                {
                    std::cout << "FAIL TO OPEN NETWORK: " << request.getDestIp().toStdString() << ":" << request.getDestPort() << std::endl;

                    SipRespond Rep(605, request);
                    sNetworkMgr->tcpSendPacket(Rep.getPacket());
                    sClientMgr->setCallRequestPeerId(0);
                }
            }
            else
            {
                SipRespond Rep(603, request);
                sNetworkMgr->tcpSendPacket(Rep.getPacket());
                sClientMgr->setCallRequestPeerId(0);
                std::cout << "SEND CALL REFUSED" << std::endl;
            }
            break;
        }
        default:
            sClientMgr->setCallRequestPeerId(0);
            break;
    }
}

void WidgetChatTab::handleByeRequest(SipRequest const& req)
{
    _callButon->setText("Call");
    addMessage(_peerName + " close call", true);
}
