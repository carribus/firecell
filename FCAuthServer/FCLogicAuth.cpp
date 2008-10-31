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
#include "FCLogicAuth.h"

FCLogicAuth::FCLogicAuth(void)
: ServiceLogicBase("FireCell Authentication Service", false)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicAuth::~FCLogicAuth(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicAuth::Start()
{
  string strEngine, strServer, strDBName, strUser, strPass;

  // load the configuration
  if ( !LoadConfig("FCAuth.conf") )
  {
    if ( HasConsole() )
      printf("Failed to load FCauth.conf configuration file\n");
    return -1;
  }

  // initialise the packet extractor
  m_pktExtractor.Prepare( __FCPACKET_DEF );

  // kick off the database object
  if ( HasConsole() )
    printf("Setting up database connection...\n");

  if ( LoadDBSettingsFromConfig(strEngine, strServer, strDBName, strUser, strPass) )
  {
    if ( !ConfigureDatabase(strEngine, strServer, strDBName, strUser, strPass) )
    {
      return -1;
    }

    RegisterDBHandler(DBQ_LOAD_ACCOUNT, OnDBJob_LoadAccount);
  }

  // connect to the router(s) that we were configured to connect to
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicAuth::Stop()
{
  DisconnectFromRouters();
  return 0;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSock = 0;
  DBJobContext* pCtx = NULL;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSock, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCMSG_LOGIN:
    {
      __FCPKT_LOGIN d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      pCtx = new DBJobContext;
      pCtx->pThis = this;
      pCtx->clientSocket = clientSock;
      pCtx->pRouter = pRouter;

      GetDatabase().ExecuteJob(DBQ_LOAD_ACCOUNT, (void*)pCtx, d.username, d.password);

      bHandled = true;
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
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
          if ( HasConsole() )
            printf("Service registered with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }
        else
        {
          // registration failed
          if ( HasConsole() )
            printf("Service failed to register with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }

      }
    }
    break;

  default:
    if ( HasConsole() )
      printf("Unknown Message Received (%ld)\n", msgID);
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDBJob_LoadAccount(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;

  if ( pSock )
  {
    PEPacket pkt;
    string strAccID;

    __FCPKT_LOGIN_RESP d;

    strAccID = resultSet.GetValue( "account_id", 0 );

    d.loginStatus = strAccID.length() > 0 ? 1 : 0;

    PEPacketHelper::CreatePacket( pkt, FCPKT_RESPONSE, FCMSG_LOGIN );
    PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_LOGIN_RESP) );

    pkt.SetFieldValue("target", &pCtx->clientSocket);

    // send the packet
    size_t dataLen = 0;
    char* pData = NULL;

    pkt.GetDataBlock( pData, dataLen );
    pSock->Send( (FCBYTE*)pData, (FCUINT)dataLen );
  }

//  delete pCtx;
//  pContext = NULL;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDBJob_LoadCharacterIDs(DBIResultSet& resultSet, void*& pContext)
{
}
