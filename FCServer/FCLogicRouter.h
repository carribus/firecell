/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

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
#ifndef _FCLOGICROUTER_H_
#define _FCLOGICROUTER_H_

#include <map>
#include <queue>
#ifdef _WIN32
  #include "../common/pthreads-win32/include/pthread.h"
#endif
#include "../common/inifile.h"
#include "interfaces/IServiceLogic.h"
#include "interfaces/ISocketServer.h"
#include "ClientSocket.h"

class FCLogicRouter : public IServiceLogic
                    , public ISocketServerSink
{
  typedef std::map<FCSOCKET, ClientSocket*> CSocketMap;
  typedef std::queue<ClientSocket*> CQueuedSocketArray;

public:
  FCLogicRouter(void);
  ~FCLogicRouter(void);

  //
  // IServiceLogic implementation
  const char* GetName()                           { return "FireCell Router Service"; }
  void Free();
  int Start();
  int Stop();
  void HasConsole(bool bHasConsole)               { m_bHasConsole = bHasConsole; }

  void StartSocketMonitor();
  void StopSocketMonitor();

  //
  // ISocketServerSink implementation
	void OnConnect(FCSOCKET s);
	void OnDisconnect(FCSOCKET s, FCDWORD dwCode);
	void OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen);

private:

  bool                LoadConfig(FCCSTR strFilename);

  // thread
  static void*        thrdSocketMonitor(void* pData);


  INIFile             m_config;
  ISocketServer*      m_pSockServer;
  bool                m_bHasConsole;
  pthread_mutex_t     m_mutexSockets;
  CSocketMap          m_mapSockets;
  pthread_mutex_t     m_mutexQueuedData;
  CQueuedSocketArray  m_arrQueuedData;

  pthread_t           m_thrdSockMon;
  bool                m_bStopSockMon;
};

#endif//_FCLOGICROUTER_H_