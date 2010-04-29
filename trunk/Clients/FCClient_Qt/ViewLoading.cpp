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
#include <QString>
#include "ViewLoading.h"
#include "FCApp.h"
#include "FCModel.h"
#include "FCNet.h"

ViewLoading::ViewLoading(QWidget* parent)
: ViewBase(parent)
{
  ui.setupUi(this);
}

///////////////////////////////////////////////////////////////////////

ViewLoading::~ViewLoading(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::setupView()
{
  FCModel& model = static_cast<FCApp*>(qApp)->model();
  FCNet& net = static_cast<FCApp*>(qApp)->network();

  connect( &net, SIGNAL(connectAttemptStarted(QString, quint16)), SLOT(onConnectAttemptStarted(QString, quint16)) );
  connect( &net, SIGNAL(connected(QString, quint16)), SLOT(onConnected(QString, quint16)) );
  connect( &net, SIGNAL(socketError(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)) );
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onConnectAttemptStarted(QString hostname, quint16 port)
{
  QString item;

  item = QString("Connecting to %1:%2").arg(hostname).arg(port);
  ui.listWidget->addItem(item);
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onConnected(QString hostName, quint16 port)
{
  QString item("Connected");
  ui.listWidget->addItem(item);
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onSocketError(QAbstractSocket::SocketError socketError)
{
  FCModel& model = static_cast<FCApp*>(qApp)->model();
  FCNet& net = static_cast<FCApp*>(qApp)->network();

  QString item("Failed to connect... ");
  
  if ( net.getRetryAttemptsLeft() > 0 )
    item += "retrying";
  else
    item += "terminating!";
  ui.listWidget->addItem(item);
}