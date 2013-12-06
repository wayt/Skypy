//
// ClientSocket.cpp for ClientSocket in /home/strohe_d/Svn/Skypy/src/shared/ClientSocket
// 
// Made by Dorian Stroher
// Login   <strohe_d@epitech.net>
// 
// Started on  Thu Dec  5 22:34:10 2013 Dorian Stroher
// Last update Fri Dec  6 01:05:21 2013 Dorian Stroher
//

#include "ClientSocket.hpp"
#include <iostream>

void	ClientSocket::readPendingDatagrams()
{
  QTextStream qout(stdout);

   while (_udpSocket->hasPendingDatagrams())
    {
      QByteArray datagram;
      datagram.resize(_udpSocket->pendingDatagramSize());
      QHostAddress sender;
      quint16 senderPort;
      _udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
      qout << datagram << " datagram received from " << sender.toString() << endl;
      //    processTheDatagram(datagram);
      WriteData(sender, senderPort, datagram);
    }
}

ClientSocket::ClientSocket(const char *Host, int HostPort)
{
  const QString test(Host);
  const QHostAddress addr(test);
  _udpSocket = new QUdpSocket(this);
  _udpSocket->bind(addr, HostPort); //HOST
  connect(_udpSocket, SIGNAL(readyRead()), this, SLOT(readPendingDatagrams()));
}

void	ClientSocket::WriteData(QHostAddress addr, quint16 Port,  QByteArray data)
{
  if (_udpSocket->writeDatagram(data, addr, Port) == -1)
    {
    qDebug("Émission du message impossible");
    _udpSocket->close();
    exit(EXIT_FAILURE);
    }
}

void ClientSocket::loop()
{
  while (_udpSocket->waitForReadyRead(-1));
}
