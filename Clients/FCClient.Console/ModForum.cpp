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
, m_currentCategoryID(0)
, m_currentThreadID(0)
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
			DisplayForumCategories();
			printf("\nSelect category (0 to exit): ");
			int nOption = _getch();

			while ( nOption < (int)'0' || nOption > (int)m_catID_max+48 )
				nOption = _getch();

			printf("\n");

			// request the threads for the chosen category
			if ( nOption == '0' )
			{
				if ( m_pSink )
					m_pSink->CloseModule(this);
			}
			else
			{
				m_currentCategoryID = nOption - 48;
				m_pServer->RequestForumThreads(nOption-48);
			}
		}
		break;

	case	ModForum::ForumThreadSelection:
		{
			DisplayCategoryThreads();
			printf("\nSelect thread (0 - back to Categories): ");
			int nOption = _getch();

			while ( nOption < (int)'0' || nOption > (int)m_catID_max+48 )
				nOption = _getch();

			printf("\n");

			// request the threads for the chosen category
			if ( nOption == '0' )
			{
				m_state = ModForum::ForumCategorySelection;
				m_currentCategoryID = 0;
				m_threads.clear();
			}
			else
			{
				m_currentThreadID = nOption - 48;
				m_pServer->RequestForumThreadDetails(m_currentCategoryID, nOption-48);
			}
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

	case	FCMSG_FORUM_GET_THREADS:
		bHandled = OnResponseForumGetThreads(pPkt, pSocket);
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

	m_catID_max = m_catID_min = 0;

	for ( FCSHORT i = 0; i < d->category_count; i++ )
	{
		c.cat_id = d->categories[i].category_id;
		c.parent_id = d->categories[i].parent_id;
		c.order = d->categories[i].order;
		c.name = d->categories[i].name;
		c.desc = d->categories[i].desc;
		m_mapCategories[c.cat_id] = c;

		if ( c.cat_id < m_catID_min )
			m_catID_min = c.cat_id;
		if ( c.cat_id > m_catID_max )
			m_catID_max = c.cat_id;
	}

	m_state = ModForum::ForumCategorySelection;

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModForum::OnResponseForumGetThreads(PEPacket* pPkt, BaseSocket* pSocket)
{
	__FCPKT_FORUM_GET_THREADS_RESP* d;
  size_t dataLen;
	Thread t;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_FORUM_GET_THREADS_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

	for ( FCULONG i = 0; i < d->thread_count; i++ )
	{
		t.thread_id = d->threads[i].thread_id;
		t.parent_id = d->threads[i].parent_id;
		t.order = d->threads[i].order;
		t.title = d->threads[i].title;
		t.author_id = d->threads[i].author_id;
		t.date_created = d->threads[i].date_created;
		t.mission_id = d->threads[i].mission_id;

		m_threads.push_back(t);
	}

	m_state = ModForum::ForumThreadSelection;

	delete [] (FCBYTE*)d;

	return true;
}

///////////////////////////////////////////////////////////////////////

void ModForum::DisplayForumCategories(FCULONG parentID)
{
	static int nLevel = 0;
	CategoryMap::iterator it = m_mapCategories.begin();
	Category* pParent = NULL;
	string name, desc;

	while ( it != m_mapCategories.end() )
	{
		if ( it->second.parent_id == parentID )
		{
			name = it->second.name;
			desc = it->second.desc;
			for ( int i = 0; i < nLevel; i++ )
				printf(" ");
			printf("[%ld] %s (%s)\n", it->second.cat_id, name.c_str(), desc.c_str());
			nLevel++;
			DisplayForumCategories( it->second.cat_id );
			nLevel--;
		}
		it++;
	}
}

///////////////////////////////////////////////////////////////////////

void ModForum::DisplayCategoryThreads()
{
	if ( m_currentCategoryID == 0 )
		return;

	ThreadVector::iterator it = m_threads.begin();

	while ( it != m_threads.end() )
	{
		printf("[%ld] %s (AuthorID: %ld | Date: %s)\n", it->thread_id, it->title.c_str(), it->author_id, it->date_created.c_str());
		it++;
	}
}