//
// Main.cpp for Main in /home/strohe_d/Svn/Skypy/src/shared/ClientSocket
// 
// Made by Dorian Stroher
// Login   <strohe_d@epitech.net>
// 
// Started on  Thu Dec  5 23:45:55 2013 Dorian Stroher
// Last update Fri Dec  6 01:03:26 2013 Dorian Stroher
//

#include "ClientSocket.hpp"

int main()
{

  /*QTextStream qout(stdout);

  QUdpSocket *udpSocket = new QUdpSocket(0);
  udpSocket->bind(4242, QUdpSocket::ShareAddress);

  while (udpSocket->waitForReadyRead(-1)) {
    while(udpSocket->hasPendingDatagrams()) {
      QByteArray datagram;
      datagram.resize(udpSocket->pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;

      udpSocket->readDatagram(datagram.data(), datagram.size(),
			      &sender, &senderPort);
      qout << "datagram received from " << sender.toString() << endl;
    }
    }*/

  ClientSocket test("127.0.0.1", 4242);
  test.loop();
  return (0);
}
