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
#ifndef _W32SOCKETSERVER_H_
#define _W32SOCKETSERVER_H_

#include <winsock2.h>
#include <ws2tcpip.h>
#include "../../common/collections/DataMgr.h"
#include "../interfaces/ISocketServer.h"

#pragma comment(lib, "ws2_32.lib")

class CW32SocketServer : public ISocketServer
{
	class CSocketPool;
	struct stClientSocket
	{
		stClientSocket()
			: s(0)
			, hEvent(0)
			, bAvailable(true)
			, pPool(NULL)
		{
		}

		FCSOCKET s;
		HANDLE hEvent;
		bool bAvailable;
		CSocketPool* pPool;
	};

	class CSocketPool
	{
	public:
		CSocketPool(CW32SocketServer* pOwner);
		~CSocketPool();

		CW32SocketServer* GetOwner()				    { return m_pOwner; }
		void SetThreadHandle(HANDLE hThrd)			{ m_hPoolThrd = hThrd; }
		HANDLE GetThreadHandle()					      { return m_hPoolThrd; }
		DWORD HasAvailableSlots()					      { return WSA_MAXIMUM_WAIT_EVENTS - m_dwSocketCount; }
		DWORD GetSocketCount()						      { return m_dwSocketCount; }
		stClientSocket* GetSocket(int nIndex);
		bool AddSocket(int nIndex, FCSOCKET s, HANDLE hEvent);
		void RemoveSocket(stClientSocket* pSocket);

	private:
		CW32SocketServer* m_pOwner;
		HANDLE m_hPoolThrd;
		DWORD m_dwSocketCount;
		stClientSocket	m_sockets[WSA_MAXIMUM_WAIT_EVENTS];
	};

public:
	CW32SocketServer(void);
	virtual ~CW32SocketServer(void);

	//////////////////////////////////////////////////////////////////////
	// PUBLIC: ISocketServer implementation
	//////////////////////////////////////////////////////////////////////
	void Initialize(LPCSTR lpszBindToAddress, short sPortToBind);
	void Start();
	void Shutdown();
	UINT GetConnectionCount();
	int RegisterSink(ISocketServerSink* pSink);
	void UnregisterSink(ISocketServerSink* pSink);

private:

	bool IsSinkRegistered(ISocketServerSink* pSink);
	bool StartListening();
	bool AcceptConnection();
	bool AddConnectionToPool(FCSOCKET s);
	void DestroyPool(CSocketPool*& pPool);

	// event handlers
	void OnDataReceived(stClientSocket* pSocket);
	void OnClientSocketClosed(stClientSocket* pSocket, DWORD dwErrorCode);

	// threads
	static unsigned WINAPI thrdListenServer(LPVOID pParam);
	static unsigned WINAPI thrdClientServer(LPVOID pParam);

	LPSTR		m_lpszServer;
	short		m_sPort;
	// listener objects
	FCSOCKET		m_sockListener;
	HANDLE		m_hListenThrd;
	HANDLE		m_hListenEvent;
	// client socket pools
	CDataMgr< CSocketPool > m_socketPools;
	// global thread control
	bool		m_bRun;
	// misc variables
	DWORD		m_dwActiveConnections;

	CDataMgr< ISocketServerSink, false > m_arrSinks;
};

#endif//_W32SOCKETSERVER_H_