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
#include <conio.h>
#include <iostream>
#include "../../common/protocol/fcprotocol.h"
#include "ModChat.h"

ModChat::ModChat(void)
: m_pSink(NULL)
, m_characterID(0)
, m_pServer(NULL)
, m_state(ModChat::Initialise)
{
}

///////////////////////////////////////////////////////////////////////

ModChat::~ModChat(void)
{
}

///////////////////////////////////////////////////////////////////////

void ModChat::QueueForAction()
{
  switch ( m_state )
  {
  case  ModChat::Initialise:
    {
      if ( m_pServer )
      {
        m_pServer->RequestChatRoomList(m_characterID);
        m_state = WaitingForResponse;
      }
    }
    break;

  case  ModChat::Running:
    {
    }
    break;

  case  ModChat::WaitingForResponse:
    break;

  default:
    printf("[Chat] Unknown state reached (%ld)\n", m_state);
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool ModChat::OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket)
{
  bool bHandled = false;

  switch ( msgID )
  {
  case  FCMSG_CHAT_LIST_ROOMS:
    bHandled = OnResponseListChatRooms(pPkt, pSocket);
    break;

  default:
    break;
  }

  if ( bHandled && m_state == ModChat::WaitingForResponse )
    m_state = ModChat::Running;

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool ModChat::OnResponseListChatRooms(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CHAT_LIST_ROOMS_RESP* d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_CHAT_LIST_ROOMS_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

  printf("Chat Rooms:\n\n");
  for ( FCULONG i = 0; i < d->numRooms; i++ )
  {
    printf("[%ld] %s (%s) (prv:%s, off:%s)\n", d->rooms[i].id
                                             , d->rooms[i].name
                                             , d->rooms[i].topic
                                             , d->rooms[i].is_private ? "true" : "false"
                                             , d->rooms[i].is_official ? "true" : "false");
  }

  delete [] (FCBYTE*)d;

  return true;
}