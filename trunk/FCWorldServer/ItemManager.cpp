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
#include "ItemManager.h"

ItemManager* ItemManager::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

ItemManager::ItemManager(void)
{
}

///////////////////////////////////////////////////////////////////////

ItemManager::~ItemManager(void)
{
  ClearItems();
  ClearItemTypes();
}

///////////////////////////////////////////////////////////////////////

ItemManager& ItemManager::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new ItemManager;
  }
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void ItemManager::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

bool ItemManager::AddItemType(FCULONG id, const string& name, const string& dbTable)
{
  ItemType* pIT = GetItemType(id);
  bool bResult = false;
  
  if ( !pIT )
  {
    pIT = new ItemType;
    pIT->SetID(id);
    pIT->SetName(name);
    pIT->SetDBTable(dbTable);
    m_lockItemTypes.LockForWrite();
    m_mapItemTypes[id] = pIT;
    m_lockItemTypes.Unlock();

    bResult = true;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

ItemType* ItemManager::GetItemType(FCULONG id)
{
  m_lockItemTypes.LockForRead();
  ItemType* pType = NULL;
  ItemTypeMap::iterator it = m_mapItemTypes.find(id);

  if ( it != m_mapItemTypes.end() )
    pType = it->second;
  m_lockItemTypes.Unlock();

  return pType;
}

///////////////////////////////////////////////////////////////////////

bool ItemManager::AddItem(FCULONG id, const string& name, FCULONG typeID, FCULONG objID, FCSHORT minLevel, FCSHORT maxLevel, FCULONG npcValue)
{
  Item* pItem = GetItem(id);
  bool bResult = false;

  if ( !pItem )
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
      pItem->SetObjectID(objID);
      pItem->SetLevels(minLevel, maxLevel);
      pItem->SetNPCValue(npcValue);
      m_lockItems.LockForWrite();
      m_mapItems[id] = pItem;
      m_lockItems.Unlock();

      bResult = true;
    }
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

Item* ItemManager::GetItem(FCULONG id)
{
  m_lockItems.LockForRead();
  Item* pItem = NULL;
  ItemMap::iterator it = m_mapItems.find(id);

  if ( it != m_mapItems.end() )
    pItem = it->second;
  m_lockItems.Unlock();

  return pItem;
}

///////////////////////////////////////////////////////////////////////

void ItemManager::ClearItemTypes()
{
  m_lockItemTypes.LockForWrite();
  ItemTypeMap::iterator it = m_mapItemTypes.begin();

  while ( it != m_mapItemTypes.end() )
  {
    delete it->second;
    it++;
  }
  m_mapItemTypes.erase( m_mapItemTypes.begin(), m_mapItemTypes.end() );
  m_lockItemTypes.Unlock();
}

///////////////////////////////////////////////////////////////////////

void ItemManager::ClearItems()
{
  m_lockItems.LockForWrite();
  ItemMap::iterator it = m_mapItems.begin();

  while ( it != m_mapItems.end() )
  {
    delete it->second;
    it++;
  }
  m_mapItems.erase( m_mapItems.begin(), m_mapItems.end() );
  m_lockItems.Unlock();
}

