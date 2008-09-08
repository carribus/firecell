#include "BaseSocket.h"

BaseSocket::BaseSocket(void)
: m_socket(0)
, m_hEvent(0)
, m_hThrd(0)
, m_bRun(false)
, m_pSink(NULL)
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 0), &wsa);
}

////////////////////////////////////////////////////////////////////////////////////

BaseSocket::~BaseSocket(void)
{
	Disconnect();
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::Attach(SOCKET s)
{
	if ( !s || m_hThrd )
		return;

	m_socket = s;

	// set the socket into non-blocking mode
	u_long uArg = 1;
	ioctlsocket( m_socket, FIONBIO, &uArg );

	m_hEvent = WSACreateEvent();

	if ( WSAEventSelect( m_socket, m_hEvent, FD_CLOSE | FD_READ | FD_WRITE | FD_CONNECT ) != 0 )
	{
		CloseHandle(m_hEvent);
		closesocket(m_socket);
		m_socket = 0;
		return;
	}

	// thread the socket's thread
	DWORD dwID = 0;
	m_hThrd = CreateThread(0, 0, thrdSocket, this, 0, &dwID);
}

////////////////////////////////////////////////////////////////////////////////////

bool BaseSocket::Create(int nType, long lEvents)
{
	if ( m_socket )
		return false;

	if ( (m_socket = socket(AF_INET, nType, IPPROTO_IP)) == INVALID_SOCKET )
	{
		return false;
	}

	// set the socket into non-blocking mode
	u_long uArg = 1;
	ioctlsocket( m_socket, FIONBIO, &uArg );

	m_hEvent = WSACreateEvent();

	if ( WSAEventSelect( m_socket, m_hEvent, lEvents ) != 0 )
	{
		CloseHandle(m_hEvent);
		closesocket(m_socket);
		m_socket = 0;
		return false;
	}

	// thread the socket's thread
	DWORD dwID = 0;
	m_hThrd = CreateThread(0, 0, thrdSocket, this, 0, &dwID);

	return (m_hThrd != NULL);
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::Connect(LPCSTR lpszServer, short sPort)
{
	if ( !m_socket )
		return;

	sockaddr_in		addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(lpszServer);
	addr.sin_port = htons(sPort);
	ZeroMemory(addr.sin_zero, sizeof(addr.sin_zero));

	int nResult = connect( m_socket, (sockaddr*) &addr, sizeof(addr) );
	if ( nResult != 0 && nResult != WSAEWOULDBLOCK )
	{
		// Problem occurred here
		DWORD dwError = WSAGetLastError();
	}
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::Disconnect()
{
	if ( !m_socket )
		return;

	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	m_socket = 0;
}

////////////////////////////////////////////////////////////////////////////////////

bool BaseSocket::Listen(short sPort)
{
	sockaddr_in		addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(sPort);
	ZeroMemory(addr.sin_zero, sizeof(addr.sin_zero));

	bind(m_socket, (sockaddr*) &addr, sizeof(sockaddr_in));
	listen(m_socket, 5);

	return true;
}

////////////////////////////////////////////////////////////////////////////////////

int BaseSocket::Send(FCBYTE* pData, int nLen)
{
	int nBytesSent = 0, nTotalSent = 0;
	FCBYTE* pOfs = pData;

	while ( nTotalSent < nLen )
	{
		nBytesSent = send( m_socket, (const char*)pOfs, nLen-nTotalSent, 0 );
		nTotalSent += nBytesSent;
		pOfs += nBytesSent;
	}

	return nTotalSent;
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::OnAccept(int nErrorCode)
{
	sockaddr addrClient;
	int nAddrSize = sizeof(sockaddr);

	// accept the new connection
	SOCKET s = WSAAccept( m_socket, &addrClient, &nAddrSize, NULL, NULL );

	if ( m_pSink )
	{
		BaseSocket* pNewSocket = new BaseSocket;
		pNewSocket->Attach(s);

		m_pSink->OnAccepted(pNewSocket, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::OnConnect(int nErrorCode)
{
	if ( m_pSink )
		m_pSink->OnConnected(this, nErrorCode);
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::OnDataReceived(int nErrorCode)
{
	char buffer[4096];
	int nBytesRead = 0;

	nBytesRead = recv(m_socket, buffer, sizeof(buffer), 0);

	if ( m_pSink )
		m_pSink->OnDataReceived(this, (FCBYTE*)buffer, nBytesRead);
}

////////////////////////////////////////////////////////////////////////////////////

void BaseSocket::OnDisconnect(int nErrorCode)
{
	if ( m_pSink )
		m_pSink->OnDisconnected(this, nErrorCode);
}

////////////////////////////////////////////////////////////////////////////////////

DWORD WINAPI BaseSocket::thrdSocket(LPVOID pParam)
{
	BaseSocket* pThis = (BaseSocket*) pParam;
	DWORD dwWaitResult = 0;
	WSANETWORKEVENTS events;

	pThis->m_bRun = true;

	while ( pThis->m_bRun )
	{
		dwWaitResult = WSAWaitForMultipleEvents( 1, &pThis->m_hEvent, TRUE, 1000, FALSE );
		if ( dwWaitResult == WSA_WAIT_EVENT_0 )
		{
			WSAEnumNetworkEvents( pThis->m_socket, pThis->m_hEvent, &events );
			if ( events.lNetworkEvents & FD_ACCEPT )
			{
				pThis->OnAccept(events.iErrorCode[FD_ACCEPT_BIT]);
			}
			if ( events.lNetworkEvents & FD_CONNECT )
			{
				pThis->OnConnect(events.iErrorCode[FD_CONNECT_BIT]);
			}
			if ( events.lNetworkEvents & FD_READ )
			{
				pThis->OnDataReceived(events.iErrorCode[FD_READ_BIT]);
			}
			if ( events.lNetworkEvents & FD_WRITE )
			{
			}
			if ( events.lNetworkEvents & FD_CLOSE )
			{
				pThis->OnDisconnect(events.iErrorCode[FD_CLOSE_BIT]);
			}
		}
	}

	return 0;
}