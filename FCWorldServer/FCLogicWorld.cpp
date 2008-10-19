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
#include <sstream>
#include "../common/protocol/fcprotocol.h"
#include "../common/PEPacket.h"
#include "../common/PEPacketHelper.h"
#include "FCLogicWorld.h"

FCLogicWorld::FCLogicWorld(void)
: m_bHasConsole(false)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicWorld::~FCLogicWorld(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicWorld::Start()
{
  // load the configuration
  if ( !LoadConfig("FCWorld.conf") )
  {
    if ( m_bHasConsole )
      printf("Failed to load FCWorld.conf configuration file\n");
    return -1;
  }

  // initialise the packet extractor
  m_pktExtractor.Prepare( __FCPACKET_DEF );
/*
  // kick off the database object
  if ( m_bHasConsole )
    printf("Setting up database connection...\n");

  if ( !ConfigureDatabase() )
  {
    return -1;
  }
*/
  // connect to the router(s) that we were configured to connect to
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicWorld::Stop()
{
  DisconnectFromRouters();
  return 0;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  if ( !nErrorCode )
  {
    m_mapRouters[ pSock->GetServer() ] = pSock;
    RegisterServiceWithRouter(pSock);
  }
  else
  {
    if ( m_bHasConsole )
      printf("Failed to connect to router (%s:%ld)\n", pSock->GetServer().c_str(), pSock->GetPort());
  }
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  // temporary code - we probably need to attempt to reconnect to the dropped router
  m_mapRouters.erase( pSock->GetServer() );
  delete pSock;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen)
{
//  if ( m_bHasConsole )
//    printf("[DATA_IN-%ld bytes]\n", nLen);

  PEPacket* pPkt = NULL;
	RouterSocket* pRouter = (RouterSocket*)pSocket;
  CBinStream<FCBYTE, true>& stream = pRouter->GetDataStream();
  size_t offset = 0;

  pRouter->AddData(pData, (FCULONG)nLen);

  if ( (pPkt = m_pktExtractor.Extract( (const char*)(FCBYTE*)stream, offset, (size_t)stream.GetLength() )) )
  {
    pPkt->DebugDump();
    stream.Delete(0, (unsigned long)offset);
    offset = 0;
    HandlePacket(pPkt, pSocket);
    delete pPkt;
  }

}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::RegisterServiceWithRouter(RouterSocket* pSock)
{
  if ( !pSock )
    return;

  PEPacket pkt;

  __FCPKT_REGISTER_SERVER d;

  d.type = ST_World;

  PEPacketHelper::CreatePacket( pkt, FCPKT_COMMAND, FCMSG_REGISTER_SERVICE );
  PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_REGISTER_SERVER) );

  // send the packet
  size_t dataLen = 0;
  char* pData = NULL;

  pkt.GetDataBlock( pData, dataLen );
  pSock->Send( (FCBYTE*)pData, (FCUINT)dataLen );
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::HandlePacket(PEPacket* pPkt, BaseSocket* pSocket)
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
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::ConnectToRouters()
{
  bool bResult = false;
  int nRouterCount = 0;
  string strKey;
  string strValue, strServer;
  short port;
  INIFile::CSection* pSection = m_config.GetSection("Routers");

  if ( pSection )
  {
    for ( nRouterCount = 0; ;nRouterCount++ )
    {
      stringstream ss;
      ss << "Router" << nRouterCount;
      strKey = ss.str();
      strValue = pSection->GetValue(strKey);
      if ( !strValue.empty() )
      {
        strServer = strValue.substr(0, strValue.find(':'));
        port = atoi( strValue.substr( strValue.find(':')+1, strValue.length() ).c_str() );

        if ( m_bHasConsole )
          printf("Connecting to router (%s:%ld)\n", strServer.c_str(), port);

        RouterSocket* pSock = new RouterSocket;
        pSock->SetServer( strServer );
        pSock->SetPort( port );
        pSock->Subscribe(this);

        pSock->Create();
        pSock->Connect(strServer.c_str(), port);
      }
      else
        break;
    }
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::DisconnectFromRouters()
{
  ServiceSocketMap::iterator it;
  RouterSocket* pRouter = NULL;

  for ( it = m_mapRouters.begin(); it != m_mapRouters.end(); it++ )
  {
    pRouter = it->second;
    pRouter->Disconnect();
    delete pRouter;
  }

  m_mapRouters.clear();
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSock = 0;
//  DBJobContext* pCtx = NULL;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSock, sizeof(FCSOCKET));
/*
  switch ( msgID )
  {
  case  FCMSG_LOGIN:
    {
      __FCPKT_LOGIN d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      pCtx = new DBJobContext;
      pCtx->clientSocket = clientSock;
      pCtx->pRouter = pRouter;

      m_db.ExecuteJob(DBQ_LOAD_ACCOUNT, (void*)pCtx, d.username, d.password);

      bHandled = true;
    }
    break;

  default:
    break;
  }
*/
  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*)pSocket;
  FCSHORT msgID = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  switch ( msgID )
  {
  case  FCMSG_REGISTER_SERVICE:
    {
      __FCPKT_REGISTER_SERVER d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      if ( d.type == ST_Auth )
      {
        if ( d.status )
        {
          // registration succeeded
          if ( m_bHasConsole )
            printf("Service registered with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }
        else
        {
          // registration failed
          if ( m_bHasConsole )
            printf("Service failed to register with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }

      }
    }
    break;

  default:
    if ( m_bHasConsole )
      printf("Unknown Message Received (%ld)\n", msgID);
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  return false;
}
