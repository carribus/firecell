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
#ifndef _MODFORUM_H_
#define _MODFORUM_H_

#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IGameModule.h"

class ModForum : public IGameModule
{
  enum ForumState
  {
    Initialise = 1,
    Running,
    WaitingForResponse,
		ForumCategorySelection
  };

	struct Category;
	typedef map<FCULONG, Category> CategoryMap;

	struct Category
	{
		FCULONG cat_id;
		FCULONG parent_id;
		FCULONG order;
		string name;
		string desc;
	};

public:
  ModForum(void);
  ~ModForum(void);

  void RegisterSink(IGameModuleSink* pSink)                       { m_pSink = pSink; }
  void SetCharacterID(FCULONG character_id)                       { m_characterID = character_id; }
  void SetServerObject(FCServerObj* pServer)                      { m_pServer = pServer; }
  void QueueForAction();
  bool OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseForumGetCategories(PEPacket* pPkt, BaseSocket* pSocket);

private:

	void DisplayForumCategories(FCULONG parentID = 0);

  IGameModuleSink*    m_pSink;
  FCULONG             m_characterID;
  FCServerObj*        m_pServer;
  ForumState          m_state;

	CategoryMap					m_mapCategories;
	FCULONG							m_catID_min;
	FCULONG							m_catID_max;
};

#endif//_MODFORUM_H_