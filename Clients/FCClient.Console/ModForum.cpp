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
#include "../../common/protocol/fcprotocol.h"
#include "ModForum.h"

ModForum::ModForum(void)
: m_pSink(NULL)
, m_characterID(0)
, m_pServer(NULL)
, m_state(ModForum::Initialise)
{
}

///////////////////////////////////////////////////////////////////////

ModForum::~ModForum(void)
{
}

///////////////////////////////////////////////////////////////////////

void ModForum::QueueForAction()
{
  switch ( m_state )
  {
  case  ModForum::Initialise:
    {
      if ( m_pServer )
        m_pServer->RequestForumThreads(m_characterID);
    }
    break;

  case  ModForum::Running:
    break;

  case  ModForum::WaitingForResponse:
    break;

  default:
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool ModForum::OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket)
{
  bool bHandled = false;

  switch ( msgID )
  {
  case  FCMSG_FORUM_GET_THREADS:
    bHandled = OnResponseForumGetThreads(pPkt, pSocket);
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool ModForum::OnResponseForumGetThreads(PEPacket* pPkt, BaseSocket* pSocket)
{
  return false;
}