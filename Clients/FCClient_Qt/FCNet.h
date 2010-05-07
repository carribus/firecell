/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _FCNET_H_
#define _FCNET_H_
#include <QObject>
#include <QByteArray>
#include <QThread>
#include <QTcpSocket>
#include "../../common/fctypes.h"
#include "../../common/PEPacket.h"
#include "../../common/PacketExtractor.h"

class FCNet : public QThread
{
  Q_OBJECT

public:
  FCNet(QObject* parent = 0);
  ~FCNet(void);

  void run()
  {
    exec();
  }

  void connectToGame(const QString& hostname, quint16 port, quint16 maxRetries = 0);
  quint16 getRetryAttemptsLeft()              { return m_retriesLeft; }

  // Latency function
  FCULONG GetLatency(FCBYTE pktType, FCSHORT msgID);

  void requestServerInfo();
  void sendLogin(QString username, QString password);
  void requestCharacterInfo();
  void sendCharacterSelection(size_t character_id);
  void requestCharacterItems(size_t character_id);
  void requestCharacterAssets(size_t character_id);
  void requestCharacterMissions(size_t character_id);

signals:
  void connectAttemptStarted(QString hostName, quint16 port);
  void connected(QString hostName, quint16 port);
  void disconnected();
  void socketError(QAbstractSocket::SocketError socketError);

  void gamePacketReceived(PEPacket* pkt);

protected slots:

  void onConnected();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError sockError);
  void onHostFound();
  void onDataReceived();

private:

  bool SendPacket(PEPacket& pkt);
  void SetLatencyAnchor(FCBYTE pktType, FCSHORT msgID);


  QTcpSocket*         m_sock;
  quint16             m_retriesLeft;
  QString             m_hostName;
  quint16             m_port;

  QByteArray          m_inBuffer;

  PacketExtractor     m_extractor;

  struct LatencyAnchor
  {
    FCBYTE pktType;
    FCSHORT msgID;
    FCULONG timestamp;
  }                   m_latencyAnchor;
};

#endif//_FCNET_H_