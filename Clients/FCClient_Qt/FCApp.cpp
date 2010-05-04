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
#include "Settings.h"
#include "ResourceManager.h"
#include "fcmainwindow.h"
#include "FCModel.h"
#include "FCPlayerModel.h"
#include "FCApp.h"

FCApp::FCApp(int& argc, char** argv)
: QApplication(argc, argv)
, m_model(NULL)
, m_net(NULL)
, m_mainWindow(NULL)
, m_pktHandler(NULL)
{
  m_state.state = AppStateNone;
  m_state.stateStep = 0;
}

///////////////////////////////////////////////////////////////////////

FCApp::~FCApp(void)
{
  if ( m_model )
    delete m_model;
  if ( m_playerModel )
    delete m_playerModel;
  if ( m_net )
    delete m_net;
  if ( m_pktHandler )
    delete m_pktHandler;
}

///////////////////////////////////////////////////////////////////////

bool FCApp::initialise()
{
  bool bResult = true;

  // create the model and network objects
  m_model = new FCModel(this);
  m_playerModel = new FCPlayerModel(this);
  m_net = new FCNet(this);
  m_net->start(QThread::NormalPriority);

  // create the packet handler object
  m_pktHandler = new PacketHandler(this);

  connect( m_net, SIGNAL(connectAttemptStarted(QString, quint16)), SLOT(onConnectAttemptStarted(QString, quint16)) );
  connect( m_net, SIGNAL(connected(QString, quint16)), SLOT(onConnected(QString, quint16)) );
  connect( m_net, SIGNAL(socketError(QAbstractSocket::SocketError)), SLOT(onSocketError(QAbstractSocket::SocketError)) );
  connect( m_net, SIGNAL(gamePacketReceived(PEPacket*)), SLOT(onGamePacketReceived(PEPacket*)) );

  connect( m_pktHandler, SIGNAL(serverInfoReceived(unsigned char, unsigned char)), SIGNAL(serverInfoReceived(unsigned char, unsigned char)) );

  // load the app settings
  Settings& settings = Settings::instance();
  settings.LoadSettings("./clientdata/settings.xml");

  return (m_model && m_net); //bResult;
}

///////////////////////////////////////////////////////////////////////

void FCApp::bootUp()
{
  Settings& settings = Settings::instance();

  // create the main window
  if ( !createMainWindow() )
  {
    qDebug() << "Failed to create main window... guess we should die!";
    quit();
  }

  ResourceManager& rm = ResourceManager::instance();
  rm.loadClientStrings("./clientdata/strings_en.xml");

  setState( AppStateLoading );

  if ( settings.GetValue("FCClient/Settings/Fullscreen", "useFullScreen").toInt() )
    m_mainWindow->showFullScreen();
  else
    m_mainWindow->show();

  // load mission strings
  setStateStep( AppState_Loading_Text );
  rm.loadMissionStrings("./clientdata/missions/missions_en.xml");

  // step through predefined states
  setStateStep( AppState_Loading_Graphics );
  setStateStep( AppState_Loading_Sounds );

  // attempt to connect to the game server
  setState(AppStateConnecting);
  QString hostName = settings.GetValue("FCClient/Settings/Server", "address");
  quint16 port = settings.GetValue("FCClient/Settings/Server", "port").toShort();
  quint16 retries = settings.GetValue("FCClient/Settings/ConnectAttempts", "count").toShort();

  qDebug() << "Connecting to server: " << hostName << ":" << port;
  m_net->connectToGame(hostName, port, retries);
}

///////////////////////////////////////////////////////////////////////

void FCApp::onConnectAttemptStarted(QString hostname, quint16 port)
{
  setStateStep( AppState_Connecting_Connecting );
}

///////////////////////////////////////////////////////////////////////

void FCApp::onConnected(QString hostName, quint16 port)
{
  setStateStep( AppState_Connecting_Connected );
  m_net->requestServerInfo();
  setStateStep( AppState_Connecting_FetchingInfo );
}

///////////////////////////////////////////////////////////////////////

void FCApp::onSocketError(QAbstractSocket::SocketError socketError)
{
  if ( m_net->getRetryAttemptsLeft() > 0 )
    setStateStep( AppState_Connecting_Retry );
  else
  {
    setStateStep( AppState_Connecting_FinalFail );

    QMessageBox* msgBox = new QMessageBox(m_mainWindow);

    msgBox->setWindowTitle("Server connection failed");
    msgBox->setText("FireCell failed to connect to the game server.\n\nExiting...");
    msgBox->exec();

    quit();
  }
}

///////////////////////////////////////////////////////////////////////

void FCApp::onGamePacketReceived(PEPacket* pPkt)
{
  FCBYTE type;
  FCSHORT msgID;
  
  pPkt->GetField("type", &type, sizeof(FCBYTE));
  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  unsigned long latency = m_net->GetLatency( FCPKT_RESPONSE, msgID );
  qDebug() << "Latency: " << latency << "ms";

  switch ( type )
  {
  case  FCPKT_COMMAND:
    m_pktHandler->onCommand(pPkt);
    break;

  case  FCPKT_RESPONSE:
    m_pktHandler->onResponse(pPkt);
    break;

  case  FCPKT_ERROR:
    m_pktHandler->onError(pPkt);
    break;
  }
}

///////////////////////////////////////////////////////////////////////

FCApp::StateInfo FCApp::getState()
{
  QMutexLocker l(&m_stateLock);

  return m_state;
}

///////////////////////////////////////////////////////////////////////

void FCApp::setState(e_AppState state)
{
  QMutexLocker l(&m_stateLock);

  StateInfo oldState = m_state;
  m_state.state = state;
  m_state.stateStep = 0;

  emit appStateChanged(m_state, oldState);
}

///////////////////////////////////////////////////////////////////////

void FCApp::setStateStep(FCSHORT stateStep)
{
  QMutexLocker l(&m_stateLock);

  StateInfo oldState = m_state;
  m_state.stateStep = stateStep;

  emit appStateChanged(m_state, oldState);
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
    connect( this, SIGNAL(appStateChanged(FCApp::StateInfo, FCApp::StateInfo)), m_mainWindow, SLOT(onAppStateChanged(FCApp::StateInfo, FCApp::StateInfo)) );
//    connect( m_model, SIGNAL(modelStateChanged(FCModel::e_ModelState, FCModel::e_ModelState)), m_mainWindow, SLOT(onModelStateChanged(FCModel::e_ModelState, FCModel::e_ModelState)) );
  }

  return (m_mainWindow != NULL);
}