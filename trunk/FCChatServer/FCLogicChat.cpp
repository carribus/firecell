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
/*
    // register the DB Job Handlers
    RegisterDBHandler(DBQ_LOAD_ACCOUNT, OnDBJob_LoadAccount);
    RegisterDBHandler(DBQ_LOAD_CHARACTER_INFO, OnDBJob_LoadCharacterInfo);
    RegisterDBHandler(DBQ_LOGIN_CHARACTER, OnDBJob_LoginCharacter);
*/
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

bool FCLogicChat::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  // switch statement on msgID

  return bHandled;
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
