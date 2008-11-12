#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class Item
{
public:

  Item(void) : m_id(0), m_typeID(0), m_objID(0), m_minLevel(0), m_maxLevel(0), m_npcValue(0)
  {
  }

  ~Item(void)
  {
  }

  void SetID(FCULONG id)              { m_id = id; }
  void SetName(string name)           { m_name = name; }
  void SetTypeID(FCULONG id)          { m_typeID = id; }
  void SetObjectID(FCULONG id)        { m_objID = id; }
  void SetLevels(FCSHORT min, FCSHORT max)        { m_minLevel = min; m_maxLevel = max; }
  void SetNPCValue(FCULONG value)     { m_npcValue = value; }

  FCULONG GetID()                     { return m_id; }
  string GetName()                    { return m_name; }
  FCULONG GetTypeID()                 { return m_typeID; }
  FCULONG GetObjectID()               { return m_objID; }
  FCSHORT GetMinLevel()               { return m_minLevel; }
  FCSHORT GetMaxLevel()               { return m_maxLevel; }
  FCULONG GetNPCValue()               { return m_npcValue; }

private:

  FCULONG       m_id;
  string        m_name;
  FCULONG       m_typeID;
  FCULONG       m_objID;
  FCSHORT       m_minLevel;
  FCSHORT       m_maxLevel;
  FCULONG       m_npcValue;
};

#endif//_ITEM_H_