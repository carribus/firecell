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
#include "../common/PacketExtractor.h"
#include "../common/PEPacketHelper.h"
#include "../common/protocol/fcprotocol.h"
#include "../common/SocketServer.h"
#include "FCLogicRouter.h"

FCLogicRouter::FCLogicRouter(void)
: m_pSockServer(NULL)
, m_bHasConsole(false)
, m_bStopSockMon(false)
{
/*
  pthread_mutexattr_t attr;
  
  // define the mutex as a recursive mutex
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
*/
  // initialize the mutex
  pthread_mutex_init(&m_mutexSockets, NULL);
  pthread_mutex_init(&m_mutexQueuedData, NULL);
  pthread_mutex_init(&m_mutexServices, NULL);
  // destroy the mutex attributes object
//  pthread_mutexattr_destroy(&attr);
}

///////////////////////////////////////////////////////////////////////

FCLogicRouter::~FCLogicRouter(void)
{
  pthread_mutex_destroy(&m_mutexQueuedData);
  pthread_mutex_destroy(&m_mutexSockets);
  pthread_mutex_destroy(&m_mutexServices);
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
    if ( m_bHasConsole )
    {
      printf("Accepting connections on port %ld\n", sPort ? sPort : 6666);
    }

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
      if ( !pSocket->IsLocked() )
      {
        // if this socket represents a service connection, remove it from the services list
        UnregisterService(pSocket);

			  m_mapSockets.erase(it);
			  delete pSocket;
      }
      else
      {
        pSocket->IsDead(true);
      }
		}
	}

  pthread_mutex_unlock(&m_mutexSockets);

}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::OnDataReceived(FCSOCKET s, FCBYTE* pData, FCINT nLen)
{
  if ( m_bHasConsole )
    printf("[DATA_IN-%ld bytes]\n", nLen);

	ClientSocket* pSocket = NULL;
	CSocketMap::iterator it = m_mapSockets.find(s);
  bool bFound = false;

  if ( it != m_mapSockets.end())
  {
    pSocket = it->second;
	  if ( pSocket )
    {
      bFound = true;
	    // add the received data to the socket's stream
	    pSocket->AddData(pData, nLen);

      pthread_mutex_lock(&m_mutexQueuedData);
      m_arrQueuedData.push(*pSocket);
      pthread_mutex_unlock(&m_mutexQueuedData);
    }
  }

  if ( bFound == false )
  {
    printf("\n*** DATA RECEIVED BUT NOT ADDED TO SOCKET STREAM\n\n");
  }
}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::HandlePacket(PEPacket* pPkt, ClientSocket* pSocket)
{
  bool bHandled = false;
  FCBYTE pktType = 0;

  pPkt->GetField("type", &pktType, sizeof(FCBYTE));

  switch ( pktType )
  {
  case  FCPKT_COMMAND:
    bHandled = OnCommand(pPkt, pSocket);
    break;

  case  FCPKT_RESPONSE:
    bHandled = OnResponse(pPkt, pSocket);
    break;

  case  FCPKT_ERROR:
    bHandled = OnError(pPkt, pSocket);
    break;
  }

  if ( !bHandled )
    ForwardPacket(pPkt, pSocket);
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::ForwardPacket(const PEPacket* pPkt, ClientSocket* pSocket)
{
  // TODO: write this code :)
}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::OnCommand(PEPacket* pPkt, ClientSocket* pSocket)
{
  FCSHORT msgID = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  switch ( msgID )
  {
  case  FCMSG_INFO_SERVER:
    {
      PEPacket* pResp = new PEPacket;
      size_t nVal;

      if ( pResp )
      {
        __FCPKT_INFO_SERVER d;

        nVal = sizeof(__FCPKT_INFO_SERVER);
        d.verMajor = 0;
        d.verMinor = 5;
        d.connectionCountRouter = m_pSockServer->GetConnectionCount();

        PEPacketHelper::CreatePacket(*pResp, FCPKT_RESPONSE, msgID);
        PEPacketHelper::SetPacketData(*pResp, (void*)&d, nVal);

        // send the packet
        size_t dataLen = 0;
        char* pData = NULL;

        pResp->GetDataBlock( pData, dataLen);
        pSocket->Send( (FCBYTE*)pData, (FCUINT)dataLen );

        delete pResp;
      }

      bHandled = true;
    }
    break;

  case  FCMSG_REGISTER_SERVICE:
    {
      size_t dataLen = 0;
      __FCPKT_REGISTER_SERVER d;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", &d, dataLen);

      RegisterService(d.type, pSocket);
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::OnResponse(PEPacket* pPkt, ClientSocket* pSocket)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::OnError(PEPacket* pPkt, ClientSocket* pSocket)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::RegisterService(ServiceType type, ClientSocket* pSocket)
{
  if ( !pSocket )
    return;

  ServiceSocket s;

  // add the socket to the 'connected services' vector
  s.pSocket = pSocket;
  s.type = type;

  pthread_mutex_lock(&m_mutexServices);
  m_arrServices.push_back(s);
  pthread_mutex_unlock(&m_mutexServices);

  if ( m_bHasConsole )
    printf("Service registered (type:%ld)\n", type);
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::UnregisterService(ClientSocket* pSocket)
{
  if ( !pSocket )
    return;

  pthread_mutex_lock(&m_mutexServices);

  CServiceSocketArray::iterator it;

  for ( it = m_arrServices.begin(); it != m_arrServices.end(); it++ )
  {
    if ( it->pSocket == pSocket )
    {
      if ( m_bHasConsole )
      {
        printf("Service unregistered (type=%ld)\n", it->type);
      }

      m_arrServices.erase(it);
      break;
    }
  }

  pthread_mutex_unlock(&m_mutexServices);
}

///////////////////////////////////////////////////////////////////////

void* FCLogicRouter::thrdSocketMonitor(void* pData)
{
  FCLogicRouter* pThis = (FCLogicRouter*) pData;
  FCSOCKET socket;
  ClientSocket* pSocket = NULL;
  PacketExtractor extractor;
  PEPacket* pPkt = NULL;
  size_t offset = 0;

  if ( !pThis )
    return NULL;

  // prepare an extraction plan for packets
  extractor.Prepare( __FCPACKET_DEF );

  while ( !pThis->m_bStopSockMon )
  {
    while ( !pThis->m_arrQueuedData.empty() )
    {
      //
      // fetch the next socked queued with data
      pthread_mutex_lock( &pThis->m_mutexQueuedData );
      socket = pThis->m_arrQueuedData.front();
      // find the ClientSocket object corresponding to this socket
      pthread_mutex_lock(&pThis->m_mutexSockets);
      CSocketMap::iterator itSock = pThis->m_mapSockets.find(socket);
      if ( itSock != pThis->m_mapSockets.end() )
        pSocket = itSock->second;
      else
        pSocket = NULL;
      pthread_mutex_unlock(&pThis->m_mutexSockets);

      pThis->m_arrQueuedData.pop();
      pthread_mutex_unlock( &pThis->m_mutexQueuedData );

      if ( pSocket )
      {
        //
        // get the data stream
        pSocket->Lock();
        NetStream& stream = pSocket->GetDataStream();

        if ( (pPkt = extractor.Extract( (const char*)(FCBYTE*)stream, offset, (size_t)stream.GetLength() )) )
        {
          pPkt->DebugDump();
          stream.Delete(0, (unsigned long)offset);
          offset = 0;
          pThis->HandlePacket(pPkt, pSocket);
        }

        pSocket->Unlock();
        if ( pSocket->IsDead() )
        {
          pThis->OnDisconnect( (FCSOCKET)(*pSocket), 0 );
        }
      }
    }

//    pthread_mutex_unlock( &pThis->m_mutexQueuedData );
    // sleep 250ms
    sched_yield();
  }

  return 0;
}

