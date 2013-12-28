#include "widgetchattab.h"
#include "widgetcontactslist.h"
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "sipPacket.hpp"
#include "audiomanager.h"
#include "clientmgr.h"

WidgetChatTab::WidgetChatTab(ContactInfo* info, QWidget *parent) :
    QWidget(parent),
    Ui::WidgetChatTab(),
    _peerId(info->getId()), _peerName(info->getName()), _peerEmail(info->getEmail()), _peerPublicIp(info->getPublicIp()), _peerPrivateIp(info->getPrivateIp())
{
    setupUi(this);
}

void WidgetChatTab::on__callButon_clicked()
{
    std::cout << "CLICKED CALL BUTTON" << std::endl;
    sClientMgr->makeCall(_peerEmail, _peerId, _peerPublicIp, _peerPrivateIp);
}

void WidgetChatTab::on__inputText_returnPressed()
{
    QString text = _inputText->text().trimmed();
    if (text.size() == 0)
        return;

    Packet pkt(CMSG_CHAT_TEXT);
    pkt << quint32(_peerId);
    pkt << text;
    sNetworkMgr->tcpSendPacket(pkt);
    _inputText->setText("");
    _chatTable->addItem("Moi: " + text);
}

void WidgetChatTab::on__sendButton_clicked()
{
    on__inputText_returnPressed();
}
