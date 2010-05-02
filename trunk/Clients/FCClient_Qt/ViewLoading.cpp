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
#include "FCPlayerModel.h"
#include "DlgLogin.h"

ViewLoading::ViewLoading(QWidget* parent)
: ViewBase(parent)
{
  ui.setupUi(this);

  const FCPlayerModel* pPlayerModel = FCAPP->playerModel();
  QObject::connect(this, SIGNAL(attemptLogin(QString, QString)), pPlayerModel, SLOT(onLogin(QString, QString)));
  QObject::connect(FCAPP, SIGNAL(serverInfoReceived(unsigned char, unsigned char)), this, SLOT(onServerInfoReceived(unsigned char, unsigned char)));
}

///////////////////////////////////////////////////////////////////////

ViewLoading::~ViewLoading(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::setupView()
{
  FCModel& model = FCAPP->model();
  FCNet& net = FCAPP->network();
  FCMainWindow* wnd = FCAPP->mainWindow();

  // connect to the model
  connect( qApp, SIGNAL(appStateChanged(FCApp::StateInfo, FCApp::StateInfo)), SLOT(onAppStateChanged(FCApp::StateInfo, FCApp::StateInfo)) );
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
      {
        openLoginDialog();
      }
      break;
    }
  }
  else
  {
    handleSubStateChange(state);
  }
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::onServerInfoReceived(unsigned char verMajor, unsigned char verMinor)
{
  QString item(" v%1.%2\n");

  item = item.arg(verMajor).arg(verMinor);
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

      case  AppState_Connecting_FetchingInfo:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_FETCHING_SERVER_INFO ) );
        break;

      case  AppState_Connecting_FinalFail:
        addLogItem( ResourceManager::instance().getClientString( STR_CONNECT_FINAL_FAIL ) );
        break;
      }
    }
    break;

  case  AppStateLogin:
    {
      switch ( state.stateStep )
      {
      case  AppState_Login_LoginFailed:
        addLogItem( ResourceManager::instance().getClientString( STR_LOGIN_FAILED ) );
        openLoginDialog();
        break;

      case  AppState_Login_LoginFailed_AccountInUse:
        addLogItem( ResourceManager::instance().getClientString( STR_LOGIN_FAILED_ACCOUNT_IN_USE ) );
        break;

      case  AppState_Login_LoginSucceeded:
        addLogItem( ResourceManager::instance().getClientString( STR_LOGIN_SUCCESSFUL ) );
        break;
      }
    }
    break;

  }
}

///////////////////////////////////////////////////////////////////////

void ViewLoading::openLoginDialog()
{
  // Allow the user to attempt to login. Cancelling the login will quit the client
  DlgLogin* pLogin = new DlgLogin(this);

  int nResult = pLogin->exec();
  if ( nResult == QDialog::Accepted)
  {
    addLogItem( ResourceManager::instance().getClientString( STR_LOGIN_ATTEMPTING ) );
    emit attemptLogin( pLogin->getUsername(), pLogin->getPassword() );
  }
  else
  {
    // we need to shutdown
    FCAPP->quit();
  }
}