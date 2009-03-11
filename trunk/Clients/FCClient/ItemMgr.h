#ifndef _ITEMMGR_H_
#define _ITEMMGR_H_

#include <map>
#include "../../common/game_objects/Item.h"

class ItemMgr
{
public:

	/*
	 *	Nested classes
	 */
	class GameItem
	{
	public:
		GameItem(Item* pItem = NULL, FCULONG count = 0)
			: m_pItem(NULL)
			, m_count(0)
		{
		}

		~GameItem()
		{
			if ( m_pItem )
				delete m_pItem;
		}

		void setItem(Item* item)									{ m_pItem = item; }
		void setCount(FCULONG count)							{ m_count = count; }
		Item* getItem()														{ return m_pItem; }
		FCULONG getCount()												{ return m_count; }

	private:
		Item* m_pItem;
		FCULONG m_count;
	};
		
	/*
	 *	Public methods
	 */
	ItemMgr(void);
	~ItemMgr(void);

	Item* addItem(FCULONG id, const std::string& name, FCULONG typeID, FCSHORT minLevel, FCSHORT maxLevel, FCULONG npcValue);
  bool getItem(FCULONG id, GameItem& item);
	void setItemCount(FCULONG id, FCULONG count);
	void clear();

private:

	/*
	 *	Private members
	 */
	typedef std::map<FCULONG, GameItem>		ItemMap;
	ItemMap m_mapItems;
};

#endif//_ITEMMGR_H_