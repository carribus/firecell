#ifndef _ITEMMGR_H_
#define _ITEMMGR_H_

#include <QObject>
#include <QReadWriteLock>
#include <map>
#include <vector>
#include "../../common/game_objects/Item.h"
//#include "../../common/PThreadMutex.h"

class ItemMgr : public QObject
{
  Q_OBJECT

public:

	/*
	 *	Nested classes
	 */
	class GameItem
	{
	public:
		GameItem(Item* pItem = NULL, FCULONG count = 0)
			: m_pItem(pItem)
			, m_count(count)
		{
		}

		~GameItem()
		{
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
	ItemMgr(QObject* parent);
	~ItemMgr(void);

	Item* addItem(FCULONG id, const std::string& name, FCULONG typeID, FCSHORT minLevel, FCSHORT maxLevel, FCULONG npcValue);
  bool getItem(FCULONG id, GameItem& item);
  FCULONG getItems(std::vector<GameItem>& items);
  FCULONG getItemCount()                                        { return (FCULONG)m_mapItems.size(); }
  FCULONG getServices(std::vector<GameItem>& items);
	void setItemCount(FCULONG id, FCULONG count);
	void clear();
  void lock();
  void unlock();

private:

	/*
	 *	Private members
	 */
	typedef std::map<FCULONG, GameItem>		ItemMap;
	ItemMap m_mapItems;
  QReadWriteLock m_lock;
};

#endif//_ITEMMGR_H_