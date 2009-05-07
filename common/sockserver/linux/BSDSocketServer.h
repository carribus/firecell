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
#ifndef _BSDSOCKETSERVER_H_
#define _BSDSOCKETSERVER_H_

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "../../fctypes.h"
#include "../../collections/DataMgr.h"
#include "../../interfaces/ISocketServer.h"

#define HANDLE void*
#define MAX_WAIT_SOCKETS  512

class BSDSocketServer : public ISocketServer
{
	class CSocketPool;
	struct stClientSocket
	{
		stClientSocket()
    : s(0)
    , bAvailable(true)
    , pPool(NULL)
		{
		}

		FCSOCKET s;
		pthread_cond_t hEvent;
		bool bAvailable;
		CSocketPool* pPool;
	};

	class CSocketPool
	{
	public:
		CSocketPool(BSDSocketServer* pOwner);
		~CSocketPool();

		BSDSocketServer* GetOwner()               { return m_pOwner; }
		void SetThreadHandle(pthread_t hThrd)     { m_hPoolThrd = hThrd; }
		pthread_t GetThreadHandle()               { return m_hPoolThrd; }
		FCDWORD HasAvailableSlots()					      { return MAX_WAIT_SOCKETS - m_dwSocketCount; }
		FCDWORD GetSocketCount()						      { return m_dwSocketCount; }
    fd_set GetFDS()                           { return m_fdsSockets; }
    int GetMaxFDS()                           { return m_nFdsMax; }
		stClientSocket* GetSocket(int nIndex);
		bool AddSocket(int nIndex, FCSOCKET s);
		void RemoveSocket(stClientSocket* pSocket);

	private:
		BSDSocketServer* m_pOwner;
		pthread_t m_hPoolThrd;
		FCDWORD m_dwSocketCount;
		stClientSocket	m_sockets[MAX_WAIT_SOCKETS];
    fd_set m_fdsSockets;
    int m_nFdsMax;
	};

public:
  BSDSocketServer(void);
  virtual ~BSDSocketServer(void);

	//////////////////////////////////////////////////////////////////////
	// PUBLIC: ISocketServer implementation
	//////////////////////////////////////////////////////////////////////
	void Initialize(FCCSTR lpszBindToAddress, FCSHORT sPortToBind);
	void Start();
	void Shutdown();
	FCUINT GetConnectionCount();
	int RegisterSink(ISocketServerSink* pSink);
	void UnregisterSink(ISocketServerSink* pSink);

private:

  bool IsSinkRegistered(ISocketServerSink* pSink);
  bool StartListening();
  bool AcceptConnection();
  bool AddConnectionToPool(FCSOCKET s);
  void DestroyPool(CSocketPool*& pPool);

  // threads
  static void* thrdListenServer(void* pParam);
  static void* thrdClientServer(void* pParam);

	// event handlers
	void OnDataReceived(stClientSocket* pSocket);
	void OnClientSocketClosed(stClientSocket* pSocket, FCDWORD dwErrorCode);

  FCSTR               m_lpszServer;
  FCSHORT             m_sPort;
  // listener objects
  FCSOCKET            m_sockListener;
  pthread_t           m_hListenThrd;
  pthread_cond_t      m_hListenEvent;
  // client socket pools
  CDataMgr< CSocketPool > m_socketPools;
  // global thread control
  bool                m_bRun;

  FCDWORD             m_dwActiveConnections;

  CDataMgr< ISocketServerSink, false >    m_arrSinks;
};

#endif//_BSDSOCKETSERVER_H_
