#ifndef SIPREQUEST_H
#define SIPREQUEST_H
#include "packet.hpp"

class sipRequest
{
public:
  explicit sipRequest(const std::string cmd, const std::string &userName, const std::string &contactName, const std::string &contactAdress, quint32 peerId): _pkt(RMSG_SIP), _cmd(cmd), _userName(userName), _contactName(contactName), _contactAdress(contactAdress) {
    _peerId = peerId;
    _pkt << cmd;
    _pkt << userName;
    _pkt << contactName;
    _pkt << contactAdress;
    _pkt << peerId;
  }
  Packet const& getPacket() const {
    return _pkt;
  }
  const std::string &getCmd() const {
    return (_cmd);
  }
  const std::string &getUserName() const {
    return (_userName);
  }
  const std::string &getContactName() const {
   return (_contactName);
  }
  const std::string &getContactAdress() const {
   return (_contactAdress);
  }
  quint32 getContactId() const {
      return _peerId;
  }
 private:
  Packet _pkt;
  const std::string _cmd;
  const std::string _userName;
  const std::string _contactName;
  const std::string _contactAdress;
  quint32 _peerId;
};

class sipRespond
{
 public:
  explicit sipRespond(int opCode, const std::string cmd, const std::string &userName, const std::string &contactName, const std::string &contactAdress, const std::string userAdress, int port, quint32 peerId): _pkt(rMSG_SIP), _cmd(cmd), _userName(userName), _contactName(contactName), _contactAdress(contactAdress), _userAdress(userAdress) {
    _opCode = opCode;
    _peerId = peerId;
    _port = port;
    _pkt << opCode;
    _pkt << cmd;
    _pkt << userName;
    _pkt << contactName;
    _pkt << contactAdress;
    _pkt << userAdress;
    _pkt << port;
    _pkt << peerId;

  }
  Packet const& getPacket() const {
    return _pkt;
  }
  int getCode() const {
    return (_opCode);
  }
  const std::string &getCmd() const {
    return (_cmd);
  }
  const std::string &getUserName() const {
    return (_userName);
  }
  const std::string &getContactName() const {
   return (_contactName);
  }
  const std::string &getContactAdress() const {
   return (_contactAdress);
  }
  const std::string &getUserAdress() const {
   return (_userAdress);
  }
  int getUserPort() const {
   return (_port);
  }
  int getContactId() const {
      return _peerId;
  }
 private:
  int _opCode;
  Packet _pkt;
  const std::string _cmd;
  const std::string _userName;
  const std::string _contactName;
  const std::string _contactAdress;
  const std::string _userAdress;
  int _port;
  quint32 _peerId;
};

#endif
