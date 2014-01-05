#include "widgetchattab.h"
#include "widgetcontactslist.h"
#include "packet.hpp"
#include "opcodemgr.h"
#include "networkmgr.h"
#include "sipPacket.hpp"
#include "audiomanager.h"
#include "clientmgr.h"
#include "widgetchattabaddwindow.h"
#include <QMessageBox>

WidgetChatTab::WidgetChatTab(ContactInfo const* info, QWidget *parent) :
    QWidget(parent),
    Ui::WidgetChatTab(),
    _tabId(info->getId()),
    _peersMap(),
    _tabType(CHAT_TAB_SINGLE),
    _addWindow(new WidgetChatTabAddWindow(this))
{
    setupUi(this);

    PeerInfo* peer = new PeerInfo();
    peer->peerId = info->getId();
    peer->peerEmail = info->getEmail();
    peer->peerName = info->getName();
    peer->peerPublicIp = info->getPublicIp();
    peer->peerPrivateIp = info->getPrivateIp();
    peer->online = info->isOnline();
    _peersMap[peer->peerId] = peer;

    _peerListWidget->hide();

    if (peer->online)
        _onlineLabel->setPixmap(QPixmap(":/images/button_icon_green"));
    else
        _onlineLabel->setPixmap(QPixmap(":/images/button_icon_red"));
}

WidgetChatTab::WidgetChatTab(quint32 id, QWidget *parent) :
    QWidget(parent),
    Ui::WidgetChatTab(),
    _tabId(id),
    _peersMap(),
    _tabType(CHAT_TAB_MULTI),
    _addWindow(new WidgetChatTabAddWindow(this))
{
    setupUi(this);
}


void WidgetChatTab::on__callButon_clicked()
{
    if (!_getOnlinePeerCount() > 0)
        return;
    if (_tabType == CHAT_TAB_SINGLE)
    {
        PeerInfo const* peer = _getFirstPeer();
        std::cout << "CLICKED CALL BUTTON" << std::endl;
        if (sClientMgr->hasActiveCallWith(peer->peerId) || sClientMgr->hasCallRequestFrom(peer->peerId))
        {
            sClientMgr->stopCall(0, peer->peerEmail, peer->peerId, peer->peerPublicIp, peer->peerPrivateIp);
            _callButon->setText("Call");
        }
        else
            sClientMgr->makeCall(0, peer->peerEmail, peer->peerId, peer->peerPublicIp, peer->peerPrivateIp);
    }
    else if (_tabType == CHAT_TAB_MULTI)
    {
        bool make = sClientMgr->hasGroupCall(getTabId());
        for (QMap<quint32, PeerInfo*>::ConstIterator itr = _peersMap.begin(); itr != _peersMap.end(); ++itr)
        {
            PeerInfo const* peer = itr.value();
            if (make)
            {
                sClientMgr->stopCall(getTabId(), peer->peerEmail, peer->peerId, peer->peerPublicIp, peer->peerPrivateIp);
                _callButon->setText("Call");
            }
            else
                sClientMgr->makeCall(getTabId(), peer->peerEmail, peer->peerId, peer->peerPublicIp, peer->peerPrivateIp);
        }
    }
}

void WidgetChatTab::on__inputText_returnPressed()
{
    QString text = _inputText->text().trimmed();
    if (text.size() == 0)
        return;

    if (_tabType == CHAT_TAB_SINGLE)
    {
        Packet pkt(CMSG_CHAT_TEXT);
        pkt << quint32(_getFirstPeer()->peerId);
        pkt << text;
        sNetworkMgr->tcpSendPacket(pkt);
        _chatTable->addItem(sClientMgr->getUserName() + ": " + text);
    }
    else if (_tabType == CHAT_TAB_MULTI)
    {
        Packet data(CMSG_GROUP_CHAT_TEXT);
        data << quint32(getTabId());
        data << text;
        sNetworkMgr->tcpSendPacket(data);
    }

    _inputText->setText("");
    _chatTable->scrollToBottom();
}

void WidgetChatTab::on__sendButton_clicked()
{
    on__inputText_returnPressed();
}

quint32 WidgetChatTab::_getOnlinePeerCount() const
{
    quint32 count = 0;
    for (QMap<quint32, PeerInfo*>::ConstIterator itr = _peersMap.begin(); itr != _peersMap.end(); ++itr)
        if (itr.value()->online)
            ++count;
    return count;
}

WidgetChatTab::PeerInfo* WidgetChatTab::_getPeerInfo(quint32 id)
{
    QMap<quint32, PeerInfo*>::Iterator itr = _peersMap.find(id);
    if (itr == _peersMap.end())
        return NULL;
    return itr.value();
}

WidgetChatTab::PeerInfo const* WidgetChatTab::getPeerInfo(quint32 id) const
{
    QMap<quint32, PeerInfo*>::ConstIterator itr = _peersMap.find(id);
    if (itr == _peersMap.end())
        return NULL;
    return itr.value();
}

WidgetChatTab::PeerInfo const* WidgetChatTab::_getFirstPeer() const
{
    QMap<quint32, PeerInfo*>::ConstIterator itr = _peersMap.begin();
    if (itr != _peersMap.end())
        return itr.value();
    return NULL;
}

void WidgetChatTab::loginContact(quint32 id)
{
    PeerInfo* peer = _getPeerInfo(id);
    if (!peer)
        return;
    peer->online = true;

    _callButon->setEnabled(true);
    _sendButton->setEnabled(true);

    addMessage(peer->peerName + " logged in !");



    if (_tabType == CHAT_TAB_SINGLE)
        _onlineLabel->setPixmap(QPixmap(":/images/button_icon_green"));
    else
    {
        QList<QListWidgetItem*> items = _peerListWidget->findItems(peer->peerName + " (" + peer->peerEmail + ")", Qt::MatchExactly);
        if (!items.empty())
        {
            QListWidgetItem* item = items.first();
            item->setIcon(QIcon(":/images/button_icon_green"));
        }
    }
}

void WidgetChatTab::logoutContact(quint32 id)
{
    PeerInfo* peer = _getPeerInfo(id);
    if (!peer)
        return;
    peer->online = false;
    bool enable = _getOnlinePeerCount() > 0;
    _callButon->setEnabled(enable);
    if (enable)
        _callButon->setText("Call");
    _sendButton->setEnabled(enable);

    addMessage(peer->peerName + " logged out ...");

    if (_tabType == CHAT_TAB_SINGLE)
        _onlineLabel->setPixmap(QPixmap(":/images/button_icon_red"));
    else
    {
        QList<QListWidgetItem*> items = _peerListWidget->findItems(peer->peerName + " (" + peer->peerEmail + ")", Qt::MatchExactly);
        if (!items.empty())
        {
            QListWidgetItem* item = items.first();
            item->setIcon(QIcon(":/images/button_icon_red"));
        }
    }
}

void WidgetChatTab::addMessage(quint32 id, QString const& msg, bool notif)
{
    QString item = msg;
    if (!notif)
    {
        if (PeerInfo const* peer = getPeerInfo(id))
            item = peer->peerName + ": " + msg;
        else if (id == sClientMgr->getAccountId())
            item = sClientMgr->getUserName() + ": " + msg;
    }
    _chatTable->addItem(item);
    _chatTable->scrollToBottom();
}

void WidgetChatTab::addMessage(QString const& msg)
{
    QString item = msg;
    _chatTable->addItem(item);
    _chatTable->scrollToBottom();
}

void WidgetChatTab::handleCallResponse(SipRespond const& resp)
{
    switch (resp.getCode())
    {
    case 100: // Forward de l'appel
        addMessage("Send call request ...");
        _callButon->setText("Stop");
        break;
    case 404: // Contact non connecte
        addMessage(resp.getDestEmail() + " isn't online");
        sClientMgr->clearCallPeers();
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 180: // Ca sonne
        addMessage("Ringing ...");
        break;
    case 200: // Ca a decroche
        addMessage("Call accepted");
        if (sAudioManager->isRunning() || sAudioManager->start())
        {
            std::cout << "RECEIV PEER ADDR: " << resp.getDestIp().toStdString() << ":" << resp.getDestPort() << std::endl;
            sNetworkMgr->setCallPeerAddr(resp.getDestId(), QHostAddress(resp.getDestIp()), resp.getDestPort());
            CallPeer* callPeer = sClientMgr->getCallPeer(resp.getDestId());
            callPeer->active = true;
            callPeer->port = resp.getDestPort();

            sAudioManager->addInputPeer(resp.getDestId());
        }
        else
            std::cout << "FAIL TO START AUDIO" << std::endl;
        break;
    case 603: // Refuse
        addMessage("Call refused");
        sClientMgr->clearCallPeers();
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 604: // Occuped
        addMessage("Occuped");
        sClientMgr->clearCallPeers();
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 605: // Peer fail to open network
        addMessage("Peer's network issue");
        sClientMgr->clearCallPeers();
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    case 606: // Peer fail to start audio
        addMessage("Peer's audio issue");
        sClientMgr->clearCallPeers();
        sNetworkMgr->quitCall();
        _callButon->setText("Call");
        break;
    }
}

void WidgetChatTab::handleCallRequest(SipRequest const& request)
{
    addMessage("Incomming call...");
    CallPeer* peer = new CallPeer(getTabId(), request.getSenderId(), request.getSenderEmail(), request.getSenderIp(), request.getSenderPort(), false);
    sClientMgr->addCallRequest(peer);
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Incomming call", "Accept call from " + request.getSenderEmail() + " ?",
                              QMessageBox::Yes | QMessageBox::No);

    std::cout << "====== CALL REQUEST CHATID: " << request.getChatId() << " =======" << std::endl;
    switch (reply)
    {
        case QMessageBox::Yes:
        case QMessageBox::No:
        {
            QHostAddress host(request.getDestIp());
            if (reply == QMessageBox::Yes)
            {
                for (quint16 selfPort = request.getSenderPort() + 1; selfPort < request.getSenderPort() + 200; ++selfPort)
                {
                    if (sNetworkMgr->addCallHostAddr(request.getSenderId(), host, selfPort))
                    {
                        if (sAudioManager->start())
                        {
                            std::cout << "SET PEER ADDR: " << request.getSenderIp().toStdString() << std::endl;
                            sNetworkMgr->setCallPeerAddr(request.getSenderId(), QHostAddress(request.getSenderIp()), request.getSenderPort());

                            std::cout << "CALL ACCEPTED, LISTEN ON " << request.getDestIp().toStdString() << ":" << request.getDestPort() << std::endl;
                            SipRespond Rep(200, request, selfPort);
                            sNetworkMgr->tcpSendPacket(Rep.getPacket());
                            if (CallPeer* peer = sClientMgr->getCallPeer(request.getSenderId()))
                                peer->active = true;
                            sAudioManager->addInputPeer(request.getSenderId());
                            _callButon->setText("Stop");
                            return;
                        }
                        else // Should send error
                        {
                            std::cout << "FAIL TO START AUDIO" << std::endl;

                            SipRespond Rep(606, request);
                            sNetworkMgr->tcpSendPacket(Rep.getPacket());
                            sClientMgr->clearCallPeers();
                        }
                    }
                }

                // Call not succeded
                {
                    std::cout << "FAIL TO OPEN NETWORK: " << request.getDestIp().toStdString() << ":" << request.getDestPort() << std::endl;

                    SipRespond Rep(605, request);
                    sNetworkMgr->tcpSendPacket(Rep.getPacket());
                    sClientMgr->clearCallPeers();
                }
            }
            else
            {
                SipRespond Rep(603, request);
                sNetworkMgr->tcpSendPacket(Rep.getPacket());
                sClientMgr->clearCallPeers();
                std::cout << "SEND CALL REFUSED" << std::endl;
            }
            break;
        }
        default:
            sClientMgr->clearCallPeers();
            break;
    }
}

void WidgetChatTab::handleByeRequest(SipRequest const& req)
{
    _callButon->setText("Call");
    if (PeerInfo const* peer = getPeerInfo(req.getSenderId()))
        addMessage(peer->peerName + " close call");
}

void WidgetChatTab::on__addButton_clicked()
{
    _addWindow->setTabId(_tabId);
    _addWindow->show();
    _addWindow->onShow();
}

void WidgetChatTab::memberJoin(PeerInfo* peer)
{
    std::cout << "PEER: " << peer->peerEmail.toStdString() << " JOIN GROUP CHAT: " << getTabId() << std::endl;
    _peersMap[peer->peerId] = peer;
    addMessage(peer->peerName + " join group");

    if (_tabType == CHAT_TAB_MULTI)
    {
        QListWidgetItem* item = new QListWidgetItem(_peerListWidget);
        item->setText(peer->peerName + " (" + peer->peerEmail + ")");
        item->setIcon(QIcon(peer->online ? ":/images/button_icon_green" : ":/images/button_icon_red"));
        _peerListWidget->addItem(item);
    }
}

QString WidgetChatTab::getTabName() const
{
    QString title = "";
    for (QMap<quint32, PeerInfo*>::ConstIterator itr = _peersMap.begin();
         itr != _peersMap.end(); ++itr)
    {
        if (title.size() > 0)
            title += ", ";
        title += itr.value()->peerName;
    }
    return title;
}

void WidgetChatTab::updateMember(PeerInfo const& peer)
{
    PeerInfo* old = _getPeerInfo(peer.peerId);
    if (!old)
        return;

    old->peerName = peer.peerName;
    old->peerEmail = peer.peerEmail;
    old->peerPublicIp = peer.peerPublicIp;
    old->peerPrivateIp = peer.peerPrivateIp;

    if (!old->online && peer.online)
        loginContact(peer.peerId);
    else if (old->online && !peer.online)
        logoutContact(peer.peerId);

    old->online = peer.online;
}

bool WidgetChatTab::removeMember(quint32 id)
{
    PeerInfo* peer = _getPeerInfo(id);
    if (!peer)
        return false;

    _peersMap.remove(id);
    addMessage(peer->peerName + " leave group");
    QList<QListWidgetItem*> items = _peerListWidget->findItems(peer->peerName + " (" + peer->peerEmail + ")", Qt::MatchExactly);
    if (!items.empty())
    {
        QListWidgetItem* item = items.first();
        _peerListWidget->removeItemWidget(item);
        delete item;
    }
    delete peer;

    if (_peerListWidget->count() == 0)
        return true;
    return false;
}
