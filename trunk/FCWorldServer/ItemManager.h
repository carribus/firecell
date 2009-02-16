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
#ifndef _ITEMMANAGER_H_
#define _ITEMMANAGER_H_

#include <map>
#include "../common/game_objects/ItemType.h"
#include "../common/game_objects/Item.h"
#include "../common/game_objects/ItemProcessor.h"
#include "../common/game_objects/ItemOS.h"
#include "../common/game_objects/ItemMemory.h"

class ItemManager
{
public:
  ItemManager(void);
  ~ItemManager(void);

  /*
   *  Item Type Functions
   */
  bool AddItemType(FCULONG id, const string& name, const string& dbTable);
  ItemType* GetItemType(FCULONG id);

  /*
   *  Item Functions
   */
  bool AddItem(FCULONG id, const string& name, FCULONG typeID, FCULONG objID, FCSHORT minLevel, FCSHORT maxLevel, FCULONG npcValue);
  Item* GetItem(FCULONG id);

private:

  void ClearItemTypes();
  void ClearItems();
  
  /*
   *  Item Types
   */
  typedef map<FCULONG, ItemType*> ItemTypeMap;
  ItemTypeMap m_mapItemTypes;

  /*
   *  Items
   */
  typedef map<FCULONG, Item*> ItemMap;
  ItemMap m_mapItems;
};

#endif//_ITEMMANAGER_H_