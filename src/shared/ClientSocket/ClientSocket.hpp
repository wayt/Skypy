//
// ClientSocket.hpp for ClientSocket in /home/strohe_d/Svn/Skypy/src/shared/ClientSocket
// 
// Made by Dorian Stroher
// Login   <strohe_d@epitech.net>
// 
// Started on  Wed Dec  4 16:28:26 2013 Dorian Stroher
// Last update Fri Dec  6 01:04:44 2013 Dorian Stroher
//

#include <QUdpSocket>
#include <QObject>

class QUdpSocket;

class ClientSocket : public QObject
{
    Q_OBJECT
public:
  ClientSocket(const char *Host, int HostPort);
  void	WriteData(QHostAddress, quint16, QByteArray);
  void	initSocket(const char *Host, int HostPort);
  void loop();
private slots:
  void	readPendingDatagrams();
private:
  QUdpSocket *_udpSocket;
};
