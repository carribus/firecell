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
#include "Settings.h"
#include "FCApp.h"
#include <QDesktopWidget>

FCApp::FCApp(int& argc, char** argv)
: QApplication(argc, argv)
, m_model(NULL)
, m_net(NULL)
, m_mainWindow(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCApp::~FCApp(void)
{
}

///////////////////////////////////////////////////////////////////////

bool FCApp::initialise()
{
  bool bResult = true;

  m_model = new FCModel(this);
  m_net = new FCNet(this);

  return (m_model && m_net); //bResult;
}

///////////////////////////////////////////////////////////////////////

void FCApp::bootUp()
{
  Settings& settings = Settings::instance();
  settings.LoadSettings("./clientData/settings.xml");

  // create the main window
  if ( !createMainWindow() )
  {
    qDebug() << "Failed to create main window... guess we should die!";
    quit();
  }

  m_model->setState( FCModel::LoadingState );

  m_mainWindow->show();

  QString hostName = settings.GetValue("FCClient/Settings/Server", "address");
  quint16 port = settings.GetValue("FCClient/Settings/Server", "port").toShort();
  quint16 retries = settings.GetValue("FCClient/Settings/ConnectAttempts", "count").toShort();

  qDebug() << "Connecting to server: " << hostName << ":" << port;
  m_net->connectToGame(hostName, port, retries);
}

///////////////////////////////////////////////////////////////////////

bool FCApp::createMainWindow()
{
  if ( m_mainWindow )
    return false;

  if ( (m_mainWindow = new FCMainWindow()) )
  {
    Settings& settings = Settings::instance();

    int fullscreen = settings.GetValue("FCClient/Settings/Fullscreen", "useFullScreen").toInt();
    int width = settings.GetValue("FCClient/Settings/Resolution", "width").toInt();
    int height = settings.GetValue("FCClient/Settings/Resolution", "height").toInt();

    if ( !fullscreen )
    {
      QDesktopWidget* pDesktop = desktop();
      QRect screenRes = pDesktop->screenGeometry();
      QRect wndRect(0, 0, width, height);

      wndRect.moveCenter( screenRes.center() );
      m_mainWindow->setGeometry(wndRect);
    }

    // connect the main window slots up as required
    connect( m_model, SIGNAL(modelStateChanged(FCModel::e_ModelState, FCModel::e_ModelState)), m_mainWindow, SLOT(onModelStateChanged(FCModel::e_ModelState, FCModel::e_ModelState)) );
  }

  return (m_mainWindow != NULL);
}