#include "../../common/game_objects/ItemType.h"
#include "../../common/game_objects/ItemProcessor.h"
#include "../../common/game_objects/ItemOS.h"
#include "../../common/game_objects/ItemMemory.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "ItemMgr.h"

ItemMgr::ItemMgr(void)
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

void ItemMgr::setItemCount(FCULONG id, FCULONG count)
{
	ItemMap::iterator it = m_mapItems.find(id);

	if ( it != m_mapItems.end() )
	{
		it->second.setCount(count);
	}
}

//////////////////////////////////////////////////////////////////////////////

void ItemMgr::clear()
{
	m_mapItems.erase( m_mapItems.begin(), m_mapItems.end() );
}