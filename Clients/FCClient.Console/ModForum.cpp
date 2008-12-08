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
#include <conio.h>
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
			{
        m_pServer->RequestForumCategories(m_characterID);
				m_state = WaitingForResponse;
			}
    }
    break;

  case  ModForum::Running:
    break;

  case  ModForum::WaitingForResponse:
    break;

	case	ModForum::ForumCategorySelection:
		{
			printf("Select category (0 to exit): ");
			int nOption = getch();

			while ( nOption < '0' && nOption > '7' )
				nOption = getch();

			// request the threads for the chosen category
		}
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
  case  FCMSG_FORUM_GET_CATEGORIES:
    bHandled = OnResponseForumGetCategories(pPkt, pSocket);
    break;

  default:
    break;
  }

	if ( bHandled && m_state == ModForum::WaitingForResponse )
		m_state = ModForum::Running;

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool ModForum::OnResponseForumGetCategories(PEPacket* pPkt, BaseSocket* pSocket)
{
	__FCPKT_FORUM_GET_CATEGORIES_RESP* d;
  size_t dataLen;
	Category c;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_FORUM_GET_CATEGORIES_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

	printf("\nForum categories\n\n", d->category_count);
	for ( FCSHORT i = 0; i < d->category_count; i++ )
	{
		c.cat_id = d->categories[i].category_id;
		c.parent_id = d->categories[i].parent_id;
		c.name = d->categories[i].name;
		c.desc = d->categories[i].desc;
		m_mapCategories[c.cat_id] = c;

		// TODO: Move the listing of the categories out of this function...
//#error you left off here
		printf("[%ld] %s - %s\n", d->categories[i].category_id, d->categories[i].name, d->categories[i].desc);
	}

	printf("\n");
	m_state = ModForum::ForumCategorySelection;

  delete [] (FCBYTE*)d;

  return true;
}