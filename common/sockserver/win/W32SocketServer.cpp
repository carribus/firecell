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
#include <process.h>
#include "W32SocketServer.h"

//////////////////////////////////////////////////////////////////////////////////////////
// CW32SocketServer::CSocketPool
//////////////////////////////////////////////////////////////////////////////////////////
CW32SocketServer::CSocketPool::CSocketPool(CW32SocketServer* pOwner)
: m_pOwner(pOwner)
, m_hPoolThrd(0)
, m_dwSocketCount(0)
{
	for ( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
	{
		m_sockets[i].pPool = this;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

CW32SocketServer::CSocketPool::~CSocketPool()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

CW32SocketServer::stClientSocket* CW32SocketServer::CSocketPool::GetSocket(int nIndex)
{ 
	if ( nIndex < 0 || nIndex >= WSA_MAXIMUM_WAIT_EVENTS ) 
		return NULL; 

	return &m_sockets[nIndex]; 
}

//////////////////////////////////////////////////////////////////////////////////////////

bool CW32SocketServer::CSocketPool::AddSocket(int nIndex, FCSOCKET s, HANDLE hEvent)
{
	if ( m_sockets[nIndex].bAvailable )
	{
		m_sockets[nIndex].bAvailable = false;
		m_sockets[nIndex].hEvent = hEvent;
		m_sockets[nIndex].s = s;

		m_dwSocketCount++;
	}
	else
	{
		WSACloseEvent(hEvent);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::CSocketPool::RemoveSocket(CW32SocketServer::stClientSocket* pSocket)
{
	for ( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
	{
		if ( &m_sockets[i] == pSocket )
		{
			pSocket->hEvent = 0;
			pSocket->s = 0;
			pSocket->bAvailable = true;

			m_dwSocketCount--;

			break;
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////////
// CW32SocketServer
//////////////////////////////////////////////////////////////////////////////////////////
CW32SocketServer::CW32SocketServer(void)
: m_lpszServer(NULL)
, m_sPort(0)
, m_hListenEvent(0)
, m_hListenThrd(0)
, m_bRun(false)
, m_sockListener(0)
, m_dwActiveConnections(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

CW32SocketServer::~CW32SocketServer(void)
{
	if ( m_lpszServer )
		free(m_lpszServer);

	WSACleanup();
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::Initialize(LPCSTR lpszBindToAddress, short sPortToBind)
{
	WSADATA wsa;
	if ( WSAStartup( MAKEWORD(2, 0), &wsa) != 0 )
	{
		// an error occurred while initialising winsock, throw an exception
		return;
	}

	if ( m_lpszServer )
  {
		free( m_lpszServer );
    m_lpszServer = NULL;
  }

	if ( lpszBindToAddress )
  {
    // an address to bind to has been specified
		m_lpszServer = _strdup(lpszBindToAddress);
  }

	m_sPort = sPortToBind;
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::Start()
{
	StartListening();
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::Shutdown()
{
	if ( m_hListenThrd )
	{
		m_bRun = false;
		SetEvent( m_hListenEvent );
		WaitForSingleObject( m_hListenThrd, INFINITE );
		WSACloseEvent(m_hListenEvent);
		CloseHandle(m_hListenThrd);
		m_hListenEvent = m_hListenThrd = 0;
	}

  delete this;
}

//////////////////////////////////////////////////////////////////////////////////////////

UINT CW32SocketServer::GetConnectionCount()
{
	return m_dwActiveConnections;
}

//////////////////////////////////////////////////////////////////////////////////////////

int CW32SocketServer::RegisterSink(ISocketServerSink* pSink)
{
	if ( !IsSinkRegistered(pSink) )
		m_arrSinks.Add(pSink);

	return m_arrSinks.GetSize();
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::UnregisterSink(ISocketServerSink* pSink)
{
	int nCount = m_arrSinks.GetSize();

	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_arrSinks.GetAt(i) == pSink )
		{
			m_arrSinks.RemoveAt(i);
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

bool CW32SocketServer::IsSinkRegistered(ISocketServerSink* pSink)
{
	int nCount = m_arrSinks.GetSize();

	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_arrSinks.GetAt(i) == pSink )
			return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool CW32SocketServer::StartListening()
{
  addrinfo hints, *servinfo, *p;
  char port[10];
  int yes = 1;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  sprintf(port, "%ld", m_sPort);
  if ( getaddrinfo(m_lpszServer, port, &hints, &servinfo) == -1 )
  {
    return false;
  }

  for ( p = servinfo; p != NULL; p = p->ai_next )
  {
    if ( !(m_sockListener = (FCSOCKET) WSASocket( p->ai_family, p->ai_socktype, p->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED )) )
    {
      return false;
    }

    setsockopt( m_sockListener, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(int) );

    if ( bind( m_sockListener, p->ai_addr, (int)p->ai_addrlen ) != 0 )
    {
      return false;
    }

    break;
  }

  // create the event object
	m_hListenEvent = WSACreateEvent();
	if ( WSAEventSelect( m_sockListener, m_hListenEvent, FD_ACCEPT | FD_CLOSE ) != 0 )
	{
		// an error occurred
		return false;
	}

	listen( m_sockListener, 5 );

	unsigned int nID = 0;
	if ( !(m_hListenThrd = (HANDLE)_beginthreadex(NULL, 0, thrdListenServer, this, 0, &nID)) )
	{
		// failed to start the thread, add to the log
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool CW32SocketServer::AcceptConnection()
{
	sockaddr addrClient;
	int addrSize = sizeof(addrClient);

	// accept the new connection
	FCSOCKET s = (FCSOCKET)WSAAccept( m_sockListener, &addrClient, &addrSize, NULL, NULL );

  if ( s == -1 )
    return false;

	// fire a notification
	int nCount = m_arrSinks.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		m_arrSinks.GetAt(i)->OnConnect( s );
	}

	// add the new connection to our pool of client connections
	if ( AddConnectionToPool(s) )
	{
		m_dwActiveConnections++;
	}
  else
  {
	  for ( int i = 0; i < nCount; i++ )
	  {
      closesocket( s );
		  m_arrSinks.GetAt(i)->OnDisconnect( s, -1 );
	  }
  }
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool CW32SocketServer::AddConnectionToPool(FCSOCKET s)
{
	if ( !s || s == INVALID_SOCKET )
		return false;

	bool bSocketAdded = false;
	int nCount = m_socketPools.GetSize();
	CSocketPool* pPool = NULL;
	stClientSocket* pSocket = NULL;

	for ( int i = 0; i < nCount && !bSocketAdded; i++ )
	{
		if ( !(pPool = m_socketPools.GetAt(i)) )
			continue;
		if ( pPool->HasAvailableSlots() )
		{
			for ( int j = 0; j < WSA_MAXIMUM_WAIT_EVENTS; j++ )
			{
				if ( (pSocket = pPool->GetSocket(j)) && pSocket->bAvailable )
				{
					if ( pPool->AddSocket( j, s, WSACreateEvent() ) )
					{
						bSocketAdded = true;
						WSAEventSelect( s, pSocket->hEvent, FD_READ | FD_WRITE | FD_CLOSE );
						break;
					}
				}
			}
		}
	}

	// check if the socket was placed
	if ( !bSocketAdded )
	{
		HANDLE hPoolThrd = 0;
		// Create a new connection pool
		pPool = new CSocketPool(this);

		if ( pPool->AddSocket( 0, s, WSACreateEvent() ) )
		{
			pSocket = pPool->GetSocket(0);
			WSAEventSelect( s, pSocket->hEvent, FD_READ | FD_WRITE | FD_CLOSE );

			hPoolThrd = (HANDLE)_beginthreadex(0, 0, thrdClientServer, pPool, 0, 0);
			pPool->SetThreadHandle(hPoolThrd);
			bSocketAdded = true;
			m_socketPools.Add(pPool);
		}
		else
			delete pPool;
	}

	return bSocketAdded;
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::DestroyPool(CSocketPool*& pPool)
{
	if ( !pPool )
		return;

	int nCount = m_socketPools.GetSize();

	for ( int i = 0; i < nCount; i++ )
	{
		if ( m_socketPools.GetAt(i) == pPool )
		{
      m_socketPools.RemoveAt(i);
      pPool = NULL;			
      break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::OnDataReceived(stClientSocket* pSocket)
{
	int nBytesRead = 0;
	char buffer[4096];

	ZeroMemory(buffer, sizeof(buffer));
	nBytesRead = recv( pSocket->s, buffer, sizeof(buffer), 0);
	if ( nBytesRead <= 0 )
	{
		// something went wrong...
		OnClientSocketClosed(pSocket, 0);
	}
	else
	{
		// fire an event to all subscribers
		int nCount = m_arrSinks.GetSize();
		for ( int i = 0; i < nCount; i++ )
		{
			m_arrSinks.GetAt(i)->OnDataReceived(pSocket->s, (BYTE*)buffer, nBytesRead);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void CW32SocketServer::OnClientSocketClosed(stClientSocket* pSocket, DWORD dwErrorCode)
{
	if ( !pSocket )
		return;

	CSocketPool* pPool = pSocket->pPool;

	// fire an event to all subscribers
	int nCount = m_arrSinks.GetSize();
	for ( int i = 0; i < nCount; i++ )
	{
		m_arrSinks.GetAt(i)->OnDisconnect(pSocket->s, dwErrorCode);
	}

	pPool->RemoveSocket(pSocket);
	if ( pPool->GetSocketCount() == 0 )
	{
		DestroyPool(pPool);
	}
	m_dwActiveConnections--;
}

//////////////////////////////////////////////////////////////////////////////////////////

unsigned WINAPI CW32SocketServer::thrdListenServer(LPVOID pParam)
{
	CW32SocketServer* pThis = (CW32SocketServer*) pParam;
	DWORD dwWaitResult = 0;
	WSANETWORKEVENTS events;

	if ( !pThis )
		return -1;

	// flag the thread to run
	pThis->m_bRun = true;

	while ( pThis->m_bRun )
	{
		dwWaitResult = WSAWaitForMultipleEvents(1, &pThis->m_hListenEvent, TRUE, 1000, FALSE); 
		if ( dwWaitResult == WSA_WAIT_EVENT_0 )
		{
			WSAEnumNetworkEvents( pThis->m_sockListener, pThis->m_hListenEvent, &events );
			if ( events.lNetworkEvents & FD_ACCEPT )
			{
				pThis->AcceptConnection();
			}
			if ( events.lNetworkEvents & FD_CLOSE )
			{
			}
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////

unsigned WINAPI CW32SocketServer::thrdClientServer(LPVOID pParam)
{
	CSocketPool* pPool = (CSocketPool*) pParam;
	CW32SocketServer* pThis = pPool->GetOwner();
	DWORD dwWaitResult = 0;
	int nIndex = 0;
	HANDLE arrHandles[WSA_MAXIMUM_WAIT_EVENTS];
	stClientSocket* arrSockets[WSA_MAXIMUM_WAIT_EVENTS];
	stClientSocket* pSocket = NULL;
	WSANETWORKEVENTS events;

	while ( pThis->m_bRun && pPool->GetSocketCount() )
	{
		// create the handle array
		ZeroMemory(arrHandles, sizeof(arrHandles));
		ZeroMemory(arrSockets, sizeof(arrSockets));
		nIndex = 0;
		for ( int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
		{
			if ( (pSocket = pPool->GetSocket(i)) && !pSocket->bAvailable )
			{
				arrSockets[nIndex] = pSocket;
				arrHandles[nIndex++] = pSocket->hEvent;
			}
		}

		// wait for an event to occur
		dwWaitResult = WSAWaitForMultipleEvents( (DWORD)nIndex, arrHandles, FALSE, 1000, FALSE );
		if ( dwWaitResult != WSA_WAIT_FAILED && (dwWaitResult >= WSA_WAIT_EVENT_0) && (dwWaitResult <= (WSA_WAIT_EVENT_0 + nIndex -1)) )
		{
			nIndex = dwWaitResult - WSA_WAIT_EVENT_0;
			if ( (pSocket = arrSockets[nIndex]) )
			{
				WSAEnumNetworkEvents( pSocket->s, pSocket->hEvent, &events );
				if ( events.lNetworkEvents & FD_READ )
				{
					// data has arrived
					pThis->OnDataReceived(pSocket);
				}
				if ( events.lNetworkEvents & FD_WRITE )
				{
					// data sent
				}
				if ( events.lNetworkEvents & FD_CLOSE )
				{
					// socket closed
					pThis->OnClientSocketClosed(pSocket, (DWORD)events.iErrorCode[FD_CLOSE_BIT]);
				}				
			}
		}
	}

	delete pPool;

	return 0;
}