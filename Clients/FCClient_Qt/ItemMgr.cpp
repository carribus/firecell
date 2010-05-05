/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

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
#include "../../common/game_objects/ItemType.h"
#include "../../common/game_objects/ItemProcessor.h"
#include "../../common/game_objects/ItemOS.h"
#include "../../common/game_objects/ItemMemory.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "ItemMgr.h"

ItemMgr::ItemMgr(QObject* parent)
: QObject(parent)
, m_lock( QReadWriteLock::Recursive )
{
}

//////////////////////////////////////////////////////////////////////////////

ItemMgr::~ItemMgr(void)
{
	clear();
}

//////////////////////////////////////////////////////////////////////////////

Item* ItemMgr::addItem(FCULONG id, const std::string& name, FCULONG typeID, FCSHORT minLevel, FCSHORT maxLevel, FCULONG npcValue)
{
	GameItem item;
  Item* pItem = NULL;

  QReadLocker l(&m_lock);
	if ( !getItem(id, item) )
	{
		switch ( typeID )
		{
		case  ItemType::Processor:
			pItem = new ItemProcessor;
			break;

		case  ItemType::OS:
			pItem = new ItemOS;
			break;

		case  ItemType::Memory:
			pItem = new ItemMemory;
			break;

		case  ItemType::Software:
			pItem = new ItemSoftware;
			break;
		}
	
		if ( pItem )
		{
			pItem->SetID(id);
			pItem->SetName(name);
			pItem->SetTypeID(typeID);
			pItem->SetLevels(minLevel, maxLevel);
			pItem->SetNPCValue(npcValue);
			m_mapItems[id] = GameItem(pItem, 0);
		}
	}

	return pItem;
}

//////////////////////////////////////////////////////////////////////////////

bool ItemMgr::getItem(FCULONG id, ItemMgr::GameItem& item)
{
  QReadLocker l(&m_lock);
	ItemMap::iterator it = m_mapItems.find(id);
	bool bResult = false;

	if ( it != m_mapItems.end() )
	{
		item = it->second;
		bResult = true;
	}

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

FCULONG ItemMgr::getItems(std::vector<GameItem>& items)
{
  QReadLocker l(&m_lock);

  ItemMap::iterator it = m_mapItems.begin();
  ItemMap::iterator limit = m_mapItems.end();
  GameItem item;

  for ( ; it != limit; ++it )
  {
    item = it->second;
    items.push_back(item);
  }

  return (FCULONG)items.size();
}

//////////////////////////////////////////////////////////////////////////////

FCULONG ItemMgr::getServices(std::vector<GameItem>& items)
{
  QReadLocker l(&m_lock);
  
  ItemMap::iterator it = m_mapItems.begin();
  ItemMap::iterator limit = m_mapItems.end();
  Item* pItem = NULL;
  GameItem gameItem;

  for ( ; it != limit; it++ )
  {
    if ( (pItem = it->second.getItem()) )
    {
      if ( pItem->GetTypeID() == ItemType::Software )
      {
        if ( ((ItemSoftware*)pItem)->IsService() )
        {
          gameItem = it->second;
          items.push_back(gameItem);
        }
      }
    }
  }

  return (FCULONG)items.size();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::setItemCount(FCULONG id, FCULONG count)
{
  QWriteLocker l(&m_lock);
	ItemMap::iterator it = m_mapItems.find(id);

	if ( it != m_mapItems.end() )
	{
		it->second.setCount(count);
	}
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::clear()
{
  QWriteLocker l(&m_lock);
	m_mapItems.erase( m_mapItems.begin(), m_mapItems.end() );
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::lock()
{
//  m_lock.Lock();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::unlock()
{
//  m_lock.Unlock();
}
