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
  // initialize the mutex
  pthread_mutex_init(&m_mutexSockets, NULL);
  pthread_mutex_init(&m_mutexQueuedData, NULL);
  pthread_mutex_init(&m_mutexServices, NULL);
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
    printf("%s connected (SOCKET: %ld)\n", pAddr, s);

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
        if ( !UnregisterService(pSocket) )
        {
          NotifyServicesOfClientDisconnect(pSocket);
        }

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
//  if ( m_bHasConsole )
//    printf("[DATA_IN-%ld bytes]\n", nLen);

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
    if ( m_bHasConsole )
      printf("\n*** ERROR: DATA RECEIVED BUT NOT ADDED TO SOCKET STREAM\n\n");
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

void FCLogicRouter::ForwardPacket(PEPacket* pPkt, ClientSocket* pSocket)
{
  ServiceType target;

  // check if we need to forward the packet first...
  pPkt->GetField("target", &target, sizeof(ServiceType));

  if ( target != ST_Router )
  {
    // locate the service connection for this packet...
    ClientSocket* pTargetNode = GetServiceConnectionByType( target );

    if ( pTargetNode )
    {
      FCSOCKET s = (FCSOCKET)*pSocket;
      // when forwarding packets, we swap the target service Identifier with the originating socket descriptor
      pPkt->SetFieldValue("target", (void*)&s);

      // send the packet on
      size_t dataLen = 0;
      char* pData = NULL;

      pPkt->GetDataBlock( pData, dataLen);
      pTargetNode->Send( (FCBYTE*)pData, (FCUINT)dataLen );
    }
    else
    {
      if ( (pTargetNode = GetClientConnection( (FCSOCKET)target )) )
      {
        target = ST_Client;
        pPkt->SetFieldValue("target", (void*)&target);

        // send the packet on
        size_t dataLen = 0;
        char* pData = NULL;

        pPkt->GetDataBlock( pData, dataLen);
        pTargetNode->Send( (FCBYTE*)pData, (FCUINT)dataLen );
      }
      else
      {
        if ( m_bHasConsole )
        {
          printf("Failed to forward packet (reason: Service or Client %ld is not registered)\n", target);
        }
      }
    }
  }
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
      bHandled = true;
    }
    break;

  default:
/*
    if ( m_bHasConsole )
      printf("Unknown Message Received (%ld)\n", msgID);
*/
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
  bool bResult = false;

  // add the socket to the 'connected services' vector
  s.pSocket = pSocket;
  s.type = type;

  pthread_mutex_lock(&m_mutexServices);
  m_arrServices.push_back(s);
  pthread_mutex_unlock(&m_mutexServices);

  bResult = true;

  SendServiceRegistrationResponse(pSocket, type, bResult);

  if ( m_bHasConsole )
    printf("Service registered (type:%s)\n", GetServiceTypeString(type).c_str());

}

///////////////////////////////////////////////////////////////////////

bool FCLogicRouter::UnregisterService(ClientSocket* pSocket)
{
  if ( !pSocket )
    return false;

  bool bResult = false;

  pthread_mutex_lock(&m_mutexServices);

  CServiceSocketArray::iterator it;

  for ( it = m_arrServices.begin(); it != m_arrServices.end(); it++ )
  {
    if ( it->pSocket == pSocket )
    {
      if ( m_bHasConsole )
      {
        printf("Service unregistered (type:%s)\n", GetServiceTypeString(it->type).c_str());
      }

      m_arrServices.erase(it);
      bResult = true;
      break;
    }
  }

  pthread_mutex_unlock(&m_mutexServices);

  return bResult;
}

///////////////////////////////////////////////////////////////////////

string FCLogicRouter::GetServiceTypeString(ServiceType type)
{
  string ret = "<unknown service>";

  switch ( type )
  {
  case  ST_Router:
    ret = "Router";
    break;

  case  ST_Auth:
    ret = "Auth";
    break;

  case  ST_World:
    ret = "World";
    break;
  }

  return ret;
}

///////////////////////////////////////////////////////////////////////

ClientSocket* FCLogicRouter::GetServiceConnectionByType(ServiceType type)
{
  ClientSocket* pResult = NULL;

  pthread_mutex_lock(&m_mutexServices);

  CServiceSocketArray::iterator it;

  for ( it = m_arrServices.begin(); it != m_arrServices.end(); it++ )
  {
    if ( it->type == type )
    {
      pResult = it->pSocket;
      break;
    }    
  }

  pthread_mutex_unlock(&m_mutexServices);

  return pResult;
}

///////////////////////////////////////////////////////////////////////

ClientSocket* FCLogicRouter::GetClientConnection(FCSOCKET s)
{
  ClientSocket* pClient = NULL;

  pthread_mutex_lock(&m_mutexSockets);

  CSocketMap::iterator it = m_mapSockets.find(s);

  if ( it != m_mapSockets.end() )
  {
    pClient = it->second;
  }

  pthread_mutex_unlock(&m_mutexSockets);

  return pClient;
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::NotifyServicesOfClientDisconnect(ClientSocket* pSocket)
{
  pthread_mutex_lock(&m_mutexServices);

  // create the packet
  PEPacket pkt;
  __FCSPKT_CLIENT_DISCONNECT d;

  d.clientSocket = (FCSOCKET)(*pSocket);
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCSMSG_CLIENT_DISCONNECT, ST_None);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(__FCSPKT_CLIENT_DISCONNECT) );

  CServiceSocketArray::iterator it;

  for ( it = m_arrServices.begin(); it != m_arrServices.end(); it++ )
  {
    pkt.SetFieldValue("target", (void*)&it->type);
    it->pSocket->Send(&pkt);
  }

  pthread_mutex_unlock(&m_mutexServices);
}

///////////////////////////////////////////////////////////////////////

void FCLogicRouter::SendServiceRegistrationResponse(ClientSocket* pSocket, ServiceType type, bool bSucceeded)
{
  PEPacket pkt;
  __FCPKT_REGISTER_SERVER d;

  d.type = type;
  d.status = bSucceeded;

  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_REGISTER_SERVICE, type);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(__FCPKT_REGISTER_SERVER) );

  // send the packet
  pSocket->Send(&pkt);
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
    if ( !pThis->m_arrQueuedData.empty() )
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

          while ( (pPkt = extractor.Extract( (const char*)(FCBYTE*)stream, offset, (size_t)stream.GetLength() )) )
          {
            pPkt->DebugDump();
            stream.Delete(0, (unsigned long)offset);
            offset = 0;
            pThis->HandlePacket(pPkt, pSocket);
            delete pPkt;
          }

          pSocket->Unlock();
          if ( pSocket->IsDead() )
          {
            pThis->OnDisconnect( (FCSOCKET)(*pSocket), 0 );
          }
        }
      }
    }
    else
    {
#ifdef _WIN32
      Sleep(50);
#else
      usleep(50000);
#endif
    }
  }

  return 0;
}

