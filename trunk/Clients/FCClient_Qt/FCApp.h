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
#ifndef _FCAPP_H_
#define _FCAPP_H_

#include <QApplication>
#include <QMutex>
#include "fcclient_consts.h"
#include "FCNet.h"
#include "PacketHandler.h"

class FCMainWindow;
class FCModel;
class FCPlayerModel;
class FCMissionMgr;

#define FCAPP static_cast<FCApp*>(qApp)

class FCApp : public QApplication
{
  Q_OBJECT

public:

  FCApp(int& argc, char** argv);
  ~FCApp(void);

  bool initialise();

  QString getResourceFolder();

  FCModel& model()                        { return *m_model; }
  FCPlayerModel* playerModel()            { return m_playerModel; }
  FCForumModel* forumModel()              { return m_forumModel; }
  FCNet& network()                        { return *m_net; }
  FCMissionMgr& missionMgr()              { return *m_missionMgr; }
  FCMainWindow* mainWindow()              { return m_mainWindow; }

  /*
   *  StateInfo structure that is passed in appStateChanged() signal
   */
	struct StateInfo
	{
		e_AppState state;
		FCSHORT stateStep;
	};

  StateInfo getState();
  void setState(e_AppState state);
  void setStateStep(FCSHORT stateStep);

signals:
  void appStateChanged(FCApp::StateInfo state, FCApp::StateInfo);

  void serverInfoReceived(unsigned char verMajor, unsigned char verMinor);

protected slots:

  void bootUp();

  void onConnectAttemptStarted(QString hostname, quint16 port);
  void onConnected(QString hostName, quint16 port);
  void onSocketError(QAbstractSocket::SocketError socketError);

  void onGamePacketReceived(PEPacket* pPkt);

private:

  bool createMainWindow();

  FCModel*            m_model;
  FCPlayerModel*      m_playerModel;
  FCForumModel*       m_forumModel;
  FCNet*              m_net;
  FCMainWindow*       m_mainWindow;
  FCMissionMgr*       m_missionMgr;

  StateInfo           m_state;
  QMutex              m_stateLock;
  PacketHandler*      m_pktHandler;

  unsigned char       m_serverVerMajor,
                      m_serverVerMinor;
};

#endif//_FCAPP_H_