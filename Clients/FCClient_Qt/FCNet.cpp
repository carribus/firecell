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
#include "FCNet.h"
#include <QHostAddress>

FCNet::FCNet(QObject* parent)
: QObject(parent)
, m_sock(NULL)
, m_retriesLeft(0)
{
  m_sock = new QTcpSocket(this);

  connect( m_sock, SIGNAL(connected()),                                 SLOT(onConnected()) );
  connect( m_sock, SIGNAL(disconnected()),                              SLOT(onDisconnected()) );
  connect( m_sock, SIGNAL(error(QAbstractSocket::SocketError)),         SLOT(onError(QAbstractSocket::SocketError)) );
  connect( m_sock, SIGNAL(hostFound()),                                 SLOT(onHostFound()) );
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