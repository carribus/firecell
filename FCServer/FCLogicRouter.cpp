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
#include <stdio.h>
#include "SocketServer.h"
#include "FCLogicRouter.h"

FCLogicRouter::FCLogicRouter(void)
: m_pSockServer(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicRouter::~FCLogicRouter(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicRouter::Start()
{
  if ( !m_pSockServer )
  {
    m_pSockServer = new SocketServer;

    m_pSockServer->Initialize(NULL, 5555);
    m_pSockServer->RegisterSink(this);
    m_pSockServer->Start();
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicRouter::Stop()
{
  if ( m_pSockServer )
  {
    m_pSockServer->Shutdown();
    m_pSockServer = NULL;
  }
  return 0;
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnConnect(FCSOCKET s)
{
	sockaddr_in addr;
	int nSize = sizeof(addr);

	getpeername(s, (sockaddr*)&addr, &nSize);
	char* pAddr = inet_ntoa( *(in_addr*) &addr.sin_addr );

  // TODO: Need to add sockets to a map for quick lookups
//	if ( m_bRunAsApp )
		printf("%s connected\n", pAddr);
/*
	ClientSocket* pSocket = new ClientSocket(s);
	m_mapSockets[s] = pSocket;
*/
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnDisconnect(FCSOCKET s, FCDWORD dwCode)
{
	sockaddr_in addr;
	int nSize = sizeof(addr);

	getpeername(s, (sockaddr*)&addr, &nSize);
	char* pAddr = inet_ntoa( *(in_addr*) &addr.sin_addr );

  // TODO: Need to remove sockets from the socket map

//	if ( m_bRunAsApp )
		printf("%s disconnected\n", pAddr);
/*
	CSocketMap::CPair* pPair = m_mapSockets.Lookup(s);
	ClientSocket* pSocket = NULL;

	if ( pPair )
	{
		if ( (pSocket = m_mapSockets.GetValueAt(pPair)) )
		{
//			CNSUserMgr::Instance().RemoveUser( pSocket->GetUser() );
			m_mapSockets.RemoveKey(s);
			delete pSocket;
		}
	}
*/
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen)
{
  printf("[DATA_IN-%ld bytes] %s\n", nLen, pData);
/*
	ClientSocket* pSocket = NULL;
	CSocketMap::CPair* pPair = m_mapSockets.Lookup(s);

	if ( !pPair )
		return;
	if ( !(pSocket = m_mapSockets.GetValueAt(pPair)) )
		return;

	// add the received data to the socket's stream
	pSocket->AddData(pData, nLen);
	m_arrQueuedData.Add(pSocket);
*/
}
