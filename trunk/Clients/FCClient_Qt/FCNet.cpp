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
#include "StdAfx.h"
#include "../../common/protocol/fcprotocol.h"
#include "../../common/time/timelib.h"
#include "../../common/PEPacketHelper.h"
#include "FCNet.h"
#include <QHostAddress>

FCNet::FCNet(QObject* parent)
//: QObject(parent)
: QThread(parent)
, m_sock(NULL)
, m_retriesLeft(0)
{
  m_sock = new QTcpSocket(this);

  connect( m_sock, SIGNAL(connected()),                                 SLOT(onConnected()) );
  connect( m_sock, SIGNAL(disconnected()),                              SLOT(onDisconnected()) );
  connect( m_sock, SIGNAL(error(QAbstractSocket::SocketError)),         SLOT(onError(QAbstractSocket::SocketError)) );
  connect( m_sock, SIGNAL(hostFound()),                                 SLOT(onHostFound()) );
  connect( m_sock, SIGNAL(readyRead()),                                 SLOT(onDataReceived()) );

	m_extractor.Prepare( __FCPACKET_DEF );

  memset(&m_latencyAnchor, 0, sizeof(LatencyAnchor));
}

///////////////////////////////////////////////////////////////////////

FCNet::~FCNet(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCNet::connectToGame(const QString& hostname, quint16 port, quint16 maxRetries)
{
  emit connectAttemptStarted(hostname, port);

  m_hostName = hostname;
  m_port = port;
  m_retriesLeft = maxRetries;
  m_sock->connectToHost(hostname, port);
}

///////////////////////////////////////////////////////////////////////

FCULONG FCNet::GetLatency(FCBYTE pktType, FCSHORT msgID)
{
  if ( msgID == m_latencyAnchor.msgID &&
       pktType != m_latencyAnchor.pktType )
  {
    return (tl_getTime() - m_latencyAnchor.timestamp);
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

void FCNet::requestServerInfo()
{
  PEPacket pkt;
  int nVal;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_INFO_SERVER);
  nVal = 0;
  PEPacketHelper::SetPacketData(pkt, &nVal, 1);
  
  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCNet::sendLogin(QString username, QString password)
{
  PEPacket pkt;
  __FCPKT_LOGIN d;

  memset(&d, 0, sizeof(d));
  strncpy(d.username, username.toLatin1(), MIN( username.length(), 64 ));
  strncpy(d.password, password.toLatin1(), MIN( password.length(), 64 )); 

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_LOGIN, ST_Auth);
  PEPacketHelper::SetPacketData(pkt, &d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCNet::requestCharacterInfo()
{
  PEPacket pkt;
  int nVal;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_GETCHARACTERS, ST_Auth);
  nVal = 0;
  PEPacketHelper::SetPacketData(pkt, &nVal, 1);
  
  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCNet::onConnected()
{
  qDebug() << "Connection established!\n\thost: " << m_sock->peerAddress().toString() << "\n\tport: " << m_sock->peerPort();

  emit connected(m_sock->peerAddress().toString(), m_sock->peerPort());
}

///////////////////////////////////////////////////////////////////////

void FCNet::onDisconnected()
{
  qDebug() << "FCNet::onDisconnected()";
}

///////////////////////////////////////////////////////////////////////

void FCNet::onError(QAbstractSocket::SocketError sockError)
{
  qDebug() << "FCNet::onError(" << sockError << ")";

  m_retriesLeft--;
  emit socketError(sockError);

  if ( m_retriesLeft )
  {
    connectToGame(m_hostName, m_port, m_retriesLeft);
  }
}

///////////////////////////////////////////////////////////////////////

void FCNet::onHostFound()
{
  qDebug() << "FCNet::onHostFound()";
}

///////////////////////////////////////////////////////////////////////

void FCNet::onDataReceived()
{
  PEPacket* pPkt = NULL;
  size_t offset = 0;
//  DataQueueItem dqi;

  // NOTE: This is probably not going to work if there are tons of packets coming in OR if we receive a partial packet...
  //       I think a partial packet will be lost with this mechanism.
  m_inBuffer.append( m_sock->readAll() );

  while ( (pPkt = m_extractor.Extract(m_inBuffer.data(), offset, (size_t)m_inBuffer.length())) )
  {
    // pass the packet on for processing
    emit gamePacketReceived(pPkt);

    // trash the 'read data' from the buffer
    if ( offset )
      m_inBuffer.remove(0, offset);

/*
    dqi.pSocket = pSocket;
    dqi.pPkt = pPkt;
    dqi.type = DataQueueItem::DQI_DataIn;

		m_mutexDataIn.Lock();
    m_qDataIn.push(dqi);
    m_mutexDataIn.Unlock();
*/
  }
}

///////////////////////////////////////////////////////////////////////

bool FCNet::SendPacket(PEPacket& pkt)
{
  FCSHORT msgID = 0;
  FCBYTE pktType = 0;
  char* pData = NULL;
  size_t blockLen = 0;
  int nRet = 0;

  pkt.GetField("msg", &msgID, sizeof(FCSHORT));  
  pkt.GetField("type", &pktType, sizeof(FCBYTE));
  pkt.GetDataBlock(pData, blockLen);

  m_sock->write( pData, blockLen );

  SetLatencyAnchor(pktType, msgID );

  return (nRet == blockLen);
}

///////////////////////////////////////////////////////////////////////

void FCNet::SetLatencyAnchor(FCBYTE pktType, FCSHORT msgID)
{
  m_latencyAnchor.pktType = pktType;
  m_latencyAnchor.msgID = msgID;
  m_latencyAnchor.timestamp = tl_getTime();
}