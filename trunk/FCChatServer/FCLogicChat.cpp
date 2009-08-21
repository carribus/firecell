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
#include "PlayerManager.h"
#include "FCLogicChat.h"

FCLogicChat::FCLogicChat(void)
: ServiceLogicBase("FC_CHAT", "FireCell Chat Service", false)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicChat::~FCLogicChat(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicChat::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicChat::Start()
{
  string strEngine, strServer, strDBName, strUser, strPass;

  // load the configuration
  if ( !LoadConfig("FCChat.conf") )
  {
    DYNLOG_ADDLOG("Failed to load FCauth.conf configuration file");
    return -1;
  }

  // initialise the packet extractor
  m_pktExtractor.Prepare( __FCPACKET_DEF );

  // kick off the database object
  DYNLOG_ADDLOG("Setting up database connection...");

  if ( LoadDBSettingsFromConfig(strEngine, strServer, strDBName, strUser, strPass) )
  {
    if ( !ConfigureDatabase(strEngine, strServer, strDBName, strUser, strPass) )
    {
      return -1;
    }

    // register the DB Job Handlers
    RegisterDBHandler(DBQ_LOAD_CHAT_ROOMS, OnDBJob_LoadChatRooms);

    // load the chat server data
    DYNLOG_ADDLOG("Loading Chat Rooms...");

    DBJobContext* pCtx = new DBJobContext;
    pCtx->pThis = this;
    GetDatabase().ExecuteJob(DBQ_LOAD_CHAT_ROOMS, (void*)pCtx);

    // we want to pause here until the item data required data is loaded
    m_condSync.WaitForSignal();
  }

  // connect to the router(s) that we were configured to connect to
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicChat::Stop()
{
  DisconnectFromRouters();
  return 0;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicChat::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  // switch statement on msgID
  switch ( msgID )
  {
  case  FCSMSG_CHARACTER_LOGGEDIN:
    {
      bHandled = OnCommandCharacterLoggedIn(pPkt, pRouter, clientSocket);
    }
    break;

  case  FCMSG_CHAT_LIST_ROOMS:
    {
      bHandled = OnCommandChatListRooms(pPkt, pRouter, clientSocket);
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicChat::OnCommandCharacterLoggedIn(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCSPKT_CHARACTER_LOGGEDIN d;
  size_t dataLen = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  // create the player's character
  Player* pPlayer = PlayerManager::instance().CreatePlayer(d.account_id, d.character_id, d.name, d.xp, d.level, d.fame_scale, d.country_id, d.city_id, d.clientSocket);

  // if we fail to create or locate an existing player object, then something went terribly wrong...
  // we need to notify the player of the problem, as well as the auth service
  if ( pPlayer )
  {
    // set the router socket to the player
    pPlayer->SetRouterSocket(pRouter);
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT( "Failed to create/find player id %ld", d.character_id ) );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicChat::OnCommandChatListRooms(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCPKT_CHAT_LIST_ROOMS d;
  size_t dataLen = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicChat::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*)pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicChat::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

void FCLogicChat::OnDBJob_LoadChatRooms(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicChat* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld Chat Rooms loaded", rowCount));

  for ( size_t i = 0; i < rowCount; i++ )
  {
    FCULONG id = resultSet.GetULongValue( "chatroom_id", i );
    string name = resultSet.GetStringValue( "chatroom_name", i );
    string topic = resultSet.GetStringValue( "chatroom_topic", i );
    FCBYTE isPrivate = resultSet.GetByteValue( "is_private", i );
    string password = resultSet.GetStringValue( "password", i );
    FCULONG minLevel = resultSet.GetULongValue( "min_character_level", i );
    FCULONG minAccType = resultSet.GetULongValue( "min_account_type", i );
    FCBYTE isOfficial = resultSet.GetByteValue( "is_official", i );

    // create the chatroom object
  }

  delete pCtx;
  pContext = NULL;

  pThis->m_condSync.Signal();
}
