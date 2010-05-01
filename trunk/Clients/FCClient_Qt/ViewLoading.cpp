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
#include "clientstrings.h"
#include "ViewLoading.h"
#include "FCApp.h"
#include "fcmainwindow.h"
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
  FCMainWindow* wnd = static_cast<FCApp*>(qApp)->mainWindow();

  // connect to the model
  connect( qApp, SIGNAL(appStateChanged(FCApp::StateInfo, FCApp::StateInfo)), SLOT(onAppStateChanged(FCApp::StateInfo, FCApp::StateInfo)) );
/*
  // connect to the network signals
  connect( &net, SIGNAL(connectAttemptStarted(QString, quint16)), SLOT(onConnectAttemptStarted(QString, quint16)) );
  connect( &net, SIGNAL(connected(QString, quint16)), SLOT(onConnected(QString, quint16)) );
  connect( &net, SIGNAL(socketError(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)) );
*/
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onAppStateChanged(FCApp::StateInfo state, FCApp::StateInfo oldState)
{
  if ( state.state != oldState.state )
  {
    switch ( state.state )
    {
    case  AppStateLoading:
      break;

    case  AppStateConnecting:
      addLogItem( ResourceManager::instance().getClientString( STR_LOAD_SEPERATOR ) );
      break;

    case  AppStateLogin:
      // show the login dialog
      break;
    }
  }
  else
  {
    handleSubStateChange(state);
  }
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onConnectAttemptStarted(QString hostname, quint16 port)
{
  QString item;

  item = ResourceManager::instance().getClientString( STR_CONNECT_SERVER );
  addLogItem(item);
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onConnected(QString hostName, quint16 port)
{
  addLogItem(ResourceManager::instance().getClientString(STR_CONNECT_OK));
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
  addLogItem(item);
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::addLogItem(QString item)
{
  QString t = ui.txtLog->text();
  t += item;
  ui.txtLog->setText(t);
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::handleSubStateChange(FCApp::StateInfo state)
{
  switch ( state.state )
  {
  case  AppStateLoading:
    {
      switch ( state.stateStep )
      {
      case  AppState_Loading_Text:
        addLogItem(ResourceManager::instance().getClientString( STR_LOAD_START ));
        addLogItem( ResourceManager::instance().getClientString( STR_LOAD_TEXT ) );
        break;

      case  AppState_Loading_Graphics:
        addLogItem( ResourceManager::instance().getClientString( STR_LOAD_GFX ) );
        break;

      case  AppState_Loading_Sounds:
        addLogItem( ResourceManager::instance().getClientString( STR_LOAD_AUDIO ) );
        break;
      }
    }
    break;

  case  AppStateConnecting:
    {
      switch ( state.stateStep )
      {
      case  AppState_Connecting_Connecting:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_SERVER ) );
        break;

      case  AppState_Connecting_Retry:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_FAIL_RETRY ) );
        break;

      case  AppState_Connecting_Connected:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_OK ) );
        break;

      case  AppState_Connecting_FinalFail:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_FINAL_FAIL ) );
        break;
      }
    }
    break;

  case  AppStateLogin:
    break;

  }
}