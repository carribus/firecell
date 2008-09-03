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

#include <pthread.h>
#include "../../common/fctypes.h"
#include "../interfaces/ISocketServer.h"

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
    
		BSDSocketServer* GetOwner()             { return m_pOwner; }
		void SetThreadHandle(HANDLE hThrd)			{ m_hPoolThrd = hThrd; }
		pthread_t GetThreadHandle()					    { return m_hPoolThrd; }
		FCDWORD HasAvailableSlots()					      { return MAX_WAIT_SOCKETS - m_dwSocketCount; }
		FCDWORD GetSocketCount()						      { return m_dwSocketCount; }
		stClientSocket* GetSocket(int nIndex);
		bool AddSocket(int nIndex, FCSOCKET s, HANDLE hEvent);
		void RemoveSocket(stClientSocket* pSocket);
    
	private:
		BSDSocketServer* m_pOwner;
		pthread_t m_hPoolThrd;
		FCDWORD m_dwSocketCount;
		stClientSocket	m_sockets[MAX_WAIT_SOCKETS];
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
  
	// event handlers
	void OnDataReceived(stClientSocket* pSocket);
	void OnClientSocketClosed(stClientSocket* pSocket, FCDWORD dwErrorCode);
  
  
  
};

#endif//_BSDSOCKETSERVER_H_