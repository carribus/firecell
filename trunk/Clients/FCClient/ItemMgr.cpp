#include "../../common/game_objects/ItemType.h"
#include "../../common/game_objects/ItemProcessor.h"
#include "../../common/game_objects/ItemOS.h"
#include "../../common/game_objects/ItemMemory.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "ItemMgr.h"

ItemMgr::ItemMgr(void)
: m_lock(true)
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

  m_lock.Lock();
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
  m_lock.Unlock();

	return pItem;
}

//////////////////////////////////////////////////////////////////////////////

bool ItemMgr::getItem(FCULONG id, ItemMgr::GameItem& item)
{
  m_lock.Lock();
	ItemMap::iterator it = m_mapItems.find(id);
	bool bResult = false;

	if ( it != m_mapItems.end() )
	{
		item = it->second;
		bResult = true;
	}
  m_lock.Unlock();

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////

FCULONG ItemMgr::getItems(std::vector<GameItem>& items)
{
  lock();

  ItemMap::iterator it = m_mapItems.begin();
  ItemMap::iterator limit = m_mapItems.end();
  GameItem item;

  for ( ; it != limit; ++it )
  {
    item = it->second;
    items.push_back(item);
  }

  unlock();

  return (FCULONG)items.size();
}

//////////////////////////////////////////////////////////////////////////////

FCULONG ItemMgr::getServices(std::vector<GameItem>& items)
{
  m_lock.Lock();
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

  m_lock.Unlock();

  return (FCULONG)items.size();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::setItemCount(FCULONG id, FCULONG count)
{
  m_lock.Lock();
	ItemMap::iterator it = m_mapItems.find(id);

	if ( it != m_mapItems.end() )
	{
		it->second.setCount(count);
	}
  m_lock.Unlock();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::clear()
{
  m_lock.Lock();
	m_mapItems.erase( m_mapItems.begin(), m_mapItems.end() );
  m_lock.Unlock();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::lock()
{
  m_lock.Lock();
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::unlock()
{
  m_lock.Unlock();
}
