#include "widgetchattab.h"
#include "widgetcontactslist.h"
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "audiomanager.h"

WidgetChatTab::WidgetChatTab(ContactInfo* info, QWidget *parent) :
    QWidget(parent),
    Ui::WidgetChatTab(),
    _peerId(info->getId()), _peerName(info->getName()), _peerEmail(info->getEmail())
{
    setupUi(this);
}

void WidgetChatTab::on__callButon_clicked()
{
  sNetworkMgr->makeCall("MOI", _peerName.toStdString(), _peerEmail.toStdString(), _peerId);
    // TODO - Dorian tu peut mettre l'appel ICI, les info du peer sont dans les attribues _peer{Id, Name, Email}
    QString host = "127.0.0.1";
    if (sAudioManager->start())
    {
        sNetworkMgr->setCallHostAddr(QHostAddress(host), AUDIO_PORT + ((_peerId % 2) == 0 ? 0 : 1));
        sNetworkMgr->setCallPeerAddr(QHostAddress(host), AUDIO_PORT + ((_peerId % 2) == 0 ? 1 : 0));
        sNetworkMgr->runCall();
        std::cout << "RUN CALL" << std::endl;
    }
    else
        std::cerr << "FAIL START: " << sAudioManager->errorText().toStdString() << std::endl;
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
