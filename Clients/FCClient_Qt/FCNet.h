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
#include <QTcpSocket>

class FCNet : public QObject
{
  Q_OBJECT

public:
  FCNet(QObject* parent = 0);
  ~FCNet(void);

  void connectToGame(const QString& hostname, quint16 port, quint16 maxRetries = 0);
  quint16 getRetryAttemptsLeft()              { return m_retriesLeft; }

signals:
  void connectAttemptStarted(QString hostName, quint16 port);
  void connected(QString hostName, quint16 port);
  void disconnected();
  void socketError(QAbstractSocket::SocketError socketError);


protected slots:

  void onConnected();
  void onDisconnected();
  void onError(QAbstractSocket::SocketError sockError);
  void onHostFound();

private:

  QTcpSocket*         m_sock;
  quint16             m_retriesLeft;
  QString             m_hostName;
  quint16             m_port;
};

#endif//_FCNET_H_