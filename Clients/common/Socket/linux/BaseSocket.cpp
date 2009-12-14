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

#include <arpa/inet.h>
#include <cstring>
#include <fcntl.h>
#include <pthread.h>
#include "BaseSocket.h"

BaseSocket::BaseSocket(void)
: m_socket(0)
, m_bRun(false)
{
}

///////////////////////////////////////////////////////////////////////

BaseSocket::~BaseSocket(void)
{
  Disconnect();
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::Attach(FCSOCKET s)
{
  if ( !s )
    return;

  m_socket = s;

	// set the socket into non-blocking mode
	FCULONG uArg = 1;
  fcntl(m_socket, F_SETFL, O_NONBLOCK);

  // create the thread
  int nRet = pthread_create( &m_hThrd, NULL, thrdSocket, (void*) this );
  if ( nRet != 0 )
  {
    // failed to start the thread
  }
}

///////////////////////////////////////////////////////////////////////

bool BaseSocket::Create(int nType)
{
  if ( !(m_socket = socket( PF_INET, nType, 0 )) )
    return false;

  // make sure we can reuse the address if we need to
  int yes = 1;
  int nRet = setsockopt( m_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int) );

  // create the thread
  nRet = pthread_create( &m_hThrd, NULL, thrdSocket, (void*) this );
  if ( nRet != 0 )
  {
    // failed to start the thread
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::Connect(string strServer, short sPort)
{
  if ( !m_socket )
    return;

  sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(strServer.c_str());
	addr.sin_port = htons(sPort);
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

	int nResult = connect( m_socket, (sockaddr*) &addr, sizeof(addr) );
	if ( nResult != 0 )
	{
		// Problem occurred here
	}
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::Disconnect()
{
  if ( !m_socket )
    return;

  shutdown(m_socket, SHUT_RDWR);
  close(m_socket);
  m_socket = 0;
}

///////////////////////////////////////////////////////////////////////

bool BaseSocket::Listen(short sPort)
{
  sockaddr_in   addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(sPort);
	memset(addr.sin_zero, 0, sizeof(addr.sin_zero));

	bind(m_socket, (sockaddr*) &addr, sizeof(sockaddr_in));
	listen(m_socket, 5);

	return true;
}

///////////////////////////////////////////////////////////////////////

int BaseSocket::Send(FCBYTE* pData, int nLen)
{
  int nBytesSent = 0, nTotalSent = 0;
  FCBYTE* pOfs = pData;

  while ( nTotalSent < nLen )
  {
    nBytesSent = send( m_socket, (const char*)pOfs, nLen - nTotalSent, 0 );
    nTotalSent += nBytesSent;
    pOfs += nBytesSent;
  }

  return nTotalSent;
}

///////////////////////////////////////////////////////////////////////

int BaseSocket::Send(PEPacket* pkt)
{
  size_t dataLen = 0;
  char* pData = NULL;

  pkt->GetDataBlock( pData, dataLen );
  return Send( (FCBYTE*)pData, (FCUINT)dataLen );
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::OnAccept(int nErrorCode)
{
  sockaddr addrClient;
  int nAddrSize = sizeof(sockaddr);

  // accept the new connection
  FCSOCKET s = (FCSOCKET)accept( m_socket, &addrClient, (socklen_t*)&nAddrSize );

  if ( m_pSink )
  {
    BaseSocket* pNewSocket = new BaseSocket;
    pNewSocket->Attach(s);

    m_pSink->OnAccepted(pNewSocket, 0);
  }
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::OnConnect(int nErrorCode)
{
  if ( m_pSink )
    m_pSink->OnConnected(this, nErrorCode);
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::OnDataReceived(int nErrorCode)
{
	char buffer[4096];
	int nBytesRead = 0;

	nBytesRead = recv(m_socket, buffer, sizeof(buffer), 0);

  if ( nBytesRead > 0 )
  {
    if ( m_pSink )
      m_pSink->OnDataReceived(this, (FCBYTE*)buffer, nBytesRead);
  }
  else
  {
    if ( m_pSink )
      m_pSink->OnConnected(this, -1);
  }
}

///////////////////////////////////////////////////////////////////////

void BaseSocket::OnDisconnect(int nErrorCode)
{
	if ( m_pSink )
		m_pSink->OnDisconnected(this, nErrorCode);
}

///////////////////////////////////////////////////////////////////////

void* BaseSocket::thrdSocket(void* pParam)
{
  BaseSocket* pThis = (BaseSocket*) pParam;
  fd_set fds;
  timeval tv;
  
  memset(&tv, 0, sizeof(timeval));

  pThis->m_bRun = true;
  FD_ZERO( &fds );

  while ( pThis->m_bRun )
  {
    FD_SET( pThis->m_socket, &fds );
    if ( select( pThis->m_socket+1, &fds, NULL, NULL, &tv) == -1 )
    {
      // something in the select broke
      break;
    }

    if ( FD_ISSET( pThis->m_socket, &fds ) )
    {
      pThis->OnDataReceived(0);
    }
  }

  return NULL;
}
