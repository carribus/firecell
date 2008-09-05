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
#include "../common/threading.h"
#include "SocketServer.h"
#include "FCLogicRouter.h"

FCLogicRouter::FCLogicRouter(void)
: m_pSockServer(NULL)
, m_bHasConsole(false)
, m_bStopSockMon(false)
{
  // initialize the mutex
  pthread_mutex_init(&m_mutexSockets, NULL);
  pthread_mutex_init(&m_mutexQueuedData, NULL);
}

///////////////////////////////////////////////////////////////////////

FCLogicRouter::~FCLogicRouter(void)
{
  pthread_mutex_destroy(&m_mutexQueuedData);
  pthread_mutex_destroy(&m_mutexSockets);
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
    LoadConfig("FCRouter.conf");
    m_pSockServer = new SocketServer;

    short sPort = m_config.GetValueShort("network", "port");
    m_pSockServer->Initialize(NULL, sPort ? sPort : 6666 );
    m_pSockServer->RegisterSink(this);
    m_pSockServer->Start();

    StartSocketMonitor();
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

void FCLogicRouter::StartSocketMonitor()
{
  m_bStopSockMon = false;
  if ( pthread_create( &m_thrdSockMon, NULL, thrdSocketMonitor, (void*)this ) != 0 )
  {
    // an error occurred...
  }
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::StopSocketMonitor()
{
  m_bStopSockMon = true;
  pthread_join( m_thrdSockMon, NULL );
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnConnect(FCSOCKET s)
{
	sockaddr_in addr;
	int nSize = sizeof(addr);

	getpeername(s, (sockaddr*)&addr, (socklen_t*)&nSize);
	char* pAddr = inet_ntoa( *(in_addr*) &addr.sin_addr );

	if ( m_bHasConsole )
		printf("%s connected\n", pAddr);

	ClientSocket* pSocket = new ClientSocket(s);

  pthread_mutex_lock(&m_mutexSockets);
	m_mapSockets[s] = pSocket;
  pthread_mutex_unlock(&m_mutexSockets);
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnDisconnect(FCSOCKET s, FCDWORD dwCode)
{
	sockaddr_in addr;
	int nSize = sizeof(addr);

	getpeername(s, (sockaddr*)&addr, (socklen_t*)&nSize);
	char* pAddr = inet_ntoa( *(in_addr*) &addr.sin_addr );

	if ( m_bHasConsole )
		printf("%s disconnected\n", pAddr);

  pthread_mutex_lock(&m_mutexSockets);

  CSocketMap::iterator it = m_mapSockets.find(s);
	ClientSocket* pSocket = NULL;

  if ( it != m_mapSockets.end() )
	{
    pSocket = it->second;
		if ( pSocket )
		{
			m_mapSockets.erase(it);
			delete pSocket;
		}
	}

  pthread_mutex_unlock(&m_mutexSockets);

}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen)
{
  if ( m_bHasConsole )
    printf("[DATA_IN-%ld bytes] %s\n", nLen, pData);

  pthread_mutex_lock(&m_mutexSockets);

	ClientSocket* pSocket = NULL;
	CSocketMap::iterator it = m_mapSockets.find(s);

  if ( it != m_mapSockets.end())
  {
    pSocket = it->second;
	  if ( pSocket )
    {
	    // add the received data to the socket's stream
	    pSocket->AddData(pData, nLen);

      pthread_mutex_lock(&m_mutexQueuedData);
      m_arrQueuedData.push(pSocket);
      pthread_mutex_unlock(&m_mutexQueuedData);
    }
  }

  pthread_mutex_unlock(&m_mutexSockets);
}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

void* FCLogicRouter::thrdSocketMonitor(void* pData)
{
  FCLogicRouter* pThis = (FCLogicRouter*) pData;
  ClientSocket* pSocket = NULL;

  if ( !pThis )
    return NULL;

  while ( !pThis->m_bStopSockMon )
  {
    while ( !pThis->m_arrQueuedData.empty() )
    {
      //
      // fetch the next socked queued with data
      pthread_mutex_lock( &pThis->m_mutexQueuedData );
      pSocket = pThis->m_arrQueuedData.front();
      pThis->m_arrQueuedData.pop();
      pthread_mutex_unlock( &pThis->m_mutexQueuedData );

      //
      // get the data stream
      NetStream& stream = pSocket->GetDataStream();


    }

    pthread_mutex_unlock( &pThis->m_mutexQueuedData );
    // sleep 250ms
    sched_yield();
  }

  return 0;
}
