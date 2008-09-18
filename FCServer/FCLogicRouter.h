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
#include "../common/threading.h"
#include "../common/inifile.h"
#include "../common/PEPacket.h"
#include "../common/interfaces/IServiceLogic.h"
#include "../common/interfaces/ISocketServer.h"
#include "../common/protocol/fcprotocol.h"
#include "../common/socket/ClientSocket.h"

class FCLogicRouter : public IServiceLogic
                    , public ISocketServerSink
{
  struct ServiceSocket
  {
    ClientSocket* pSocket;
    ServiceType type;
  };

  typedef std::map<FCSOCKET, ClientSocket*> CSocketMap;
  typedef std::vector<ServiceSocket> CServiceSocketArray;
  typedef std::queue<FCSOCKET> CQueuedSocketArray;

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
  void                HandlePacket(PEPacket* pPkt, ClientSocket* pSocket);
  void                ForwardPacket(const PEPacket* pPkt, ClientSocket* pSocket);

  bool                OnCommand(PEPacket* pPkt, ClientSocket* pSocket);
  bool                OnResponse(PEPacket* pPkt, ClientSocket* pSocket);
  bool                OnError(PEPacket* pPkt, ClientSocket* pSocket);

  void                RegisterService(ServiceType type, ClientSocket* pSocket);
  void                UnregisterService(ClientSocket* pSocket);

  void                SendServiceRegistrationResponse(ClientSocket* pSocket, ServiceType type, bool bSucceeded);

  // thread
  static void*        thrdSocketMonitor(void* pData);

  //
  // Attributes
  //
  INIFile             m_config;
  ISocketServer*      m_pSockServer;
  bool                m_bHasConsole;
  pthread_mutex_t     m_mutexSockets;
  CSocketMap          m_mapSockets;
  pthread_mutex_t     m_mutexQueuedData;
  CQueuedSocketArray  m_arrQueuedData;

  pthread_mutex_t     m_mutexServices;
  CServiceSocketArray m_arrServices;

  pthread_t           m_thrdSockMon;
  bool                m_bStopSockMon;
};

#endif//_FCLOGICROUTER_H_