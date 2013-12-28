#ifndef SIPREQUEST_H
#define SIPREQUEST_H

#include "opcodemgr.h"
#include "packet.hpp"

class SipRequest
{
public:
    explicit SipRequest(QString const& cmd, QString const& senderEmail, quint32 senderId, QString const& senderIp, quint32 senderPort, QString const& destEmail, quint32 destId, QString const& destIp, quint32 destPort) :
        _pkt(RMSG_SIP), _cmd(cmd), _senderEmail(senderEmail), _senderId(senderId), _senderIp(senderIp), _senderPort(senderPort),
        _destEmail(destEmail), _destId(destId), _destIp(destIp), _destPort(destPort)
    {
        _pkt << _cmd;
        _pkt << _senderEmail;
        _pkt << quint32(_senderId);
        _pkt << _senderIp;
        _pkt << quint32(_senderPort);
        _pkt << _destEmail;
        _pkt << quint32(_destId);
        _pkt << _destIp;
        _pkt << quint32(_destPort);
    }
    Packet const& getPacket() const {
        return _pkt;
    }
    QString const& getCmd() const {
        return _cmd;
    }
    QString const& getSenderEmail() const {
        return _senderEmail;
    }
    quint32 getSenderId() const {
        return _senderId;
    }
    QString const& getSenderIp() const {
        return _senderIp;
    }
    quint32 getSenderPort() const {
        return _senderPort;
    }
    QString const& getDestEmail() const {
        return _destEmail;
    }
    quint32 getDestId() const {
        return _destId;
    }
    QString const& getDestIp() const {
        return _destIp;
    }
    quint32 getDestPort() const {
        return _destPort;
    }
private:
    Packet _pkt;
    QString const _cmd;
    QString const _senderEmail;
    quint32 _senderId;
    QString const _senderIp;
    quint32 _senderPort;
    QString const _destEmail;
    quint32 _destId;
    QString const _destIp;
    quint32 _destPort;
};

class SipRespond
{
public:
    explicit SipRespond(quint32 code, QString const& cmd, QString const& senderEmail, quint32 senderId, QString const& senderIp, quint32 senderPort, QString const& destEmail, quint32 destId, QString const& destIp, quint32 destPort) :
        _pkt(rMSG_SIP), _code(code), _cmd(cmd),
        _senderEmail(senderEmail), _senderId(senderId), _senderIp(senderIp), _senderPort(senderPort),
        _destEmail(destEmail), _destId(destId), _destIp(destIp), _destPort(destPort)
    {
        _pkt << _code;
        _pkt << _cmd;
        _pkt << _senderEmail;
        _pkt << quint32(_senderId);
        _pkt << _senderIp;
        _pkt << quint32(_senderPort);
        _pkt << _destEmail;
        _pkt << _destId;
        _pkt << _destIp;
        _pkt << _destPort;

    }
    SipRespond(quint32 code, SipRequest const& req) :
        _pkt(rMSG_SIP), _code(code), _cmd(req.getCmd()),
        _senderEmail(req.getSenderEmail()), _senderId(req.getSenderId()), _senderIp(req.getSenderIp()), _senderPort(req.getSenderPort()),
        _destEmail(req.getDestEmail()), _destId(req.getDestId()), _destIp(req.getDestIp()), _destPort(req.getDestPort())
    {
        _pkt << _code;
        _pkt << _cmd;
        _pkt << _senderEmail;
        _pkt << quint32(_senderId);
        _pkt << _senderIp;
        _pkt << quint32(_senderPort);
        _pkt << _destEmail;
        _pkt << _destId;
        _pkt << _destIp;
        _pkt << _destPort;

    }

    Packet const& getPacket() const {
        return _pkt;
    }
    quint32 getCode() const {
        return _code;
    }
    QString const& getCmd() const {
        return _cmd;
    }
    QString const& getSenderEmail() const {
        return _senderEmail;
    }
    quint32 getSenderId() const {
        return _senderId;
    }
    QString const& getSenderIp() const {
        return _senderIp;
    }
    quint32 getSenderPort() const {
        return _senderPort;
    }
    QString const& getDestEmail() const {
        return _destEmail;
    }
    quint32 getDestId() const {
        return _destId;
    }
    QString const& getDestIp() const {
        return _destIp;
    }
    quint32 getDestPort() const {
        return _destPort;
    }
private:
    Packet _pkt;
    quint32 _code;
    QString const _cmd;
    QString const _senderEmail;
    quint32 _senderId;
    QString const _senderIp;
    quint32 _senderPort;
    QString const _destEmail;
    quint32 _destId;
    QString const _destIp;
    quint32 _destPort;
};

#endif
