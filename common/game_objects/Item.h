#ifndef _ITEM_H_
#define _ITEM_H_

#include <string>
#include "../fctypes.h"

using namespace std;

/**
 *  \brief The Item class is the base class for all items in the game
 *  \ingroup gameobj Game Objects
 *
 *  The Item class object is shared between the server and client code bases. The big difference between the 2 consumers
 *  is that the server codebase makes use of the ObjectID in the item, while the client codebase does not use it.
 */
class Item
{
public:

  Item(void) 
    : m_id(0)
    , m_typeID(0)
#ifndef _FCCLIENT_COMPILE_
    , m_objID(0)
#endif//_FCCLIENT_COMPILE_
    , m_minLevel(0)
    , m_maxLevel(0)
    , m_npcValue(0)
  {
  }

  ~Item(void)
  {
  }

  void SetItemInfo(FCULONG id, 
                   const std::string name, 
                   FCULONG typeID, 
#ifndef _FCCLIENT_COMPILE_
                   FCULONG objID, 
#endif//_FCCLIENT_COMPILE_
                   FCSHORT minLevel, 
                   FCSHORT maxLevel, 
                   FCULONG npcValue)
  {
    m_id = id;
    m_name = name;
    m_typeID = typeID;
#ifndef _FCCLIENT_COMPILE_
    m_objID = objID;
#endif//_FCCLIENT_COMPILE_
    m_minLevel = minLevel;
    m_maxLevel = maxLevel;
    m_npcValue = npcValue;
  }

  void SetID(FCULONG id)              { m_id = id; }
  void SetName(std::string name)      { m_name = name; }
  void SetTypeID(FCULONG id)          { m_typeID = id; }
#ifndef _FCCLIENT_COMPILE_
  void SetObjectID(FCULONG id)        { m_objID = id; }
#endif//_FCCLIENT_COMPILE_
  void SetLevels(FCSHORT min, FCSHORT max)        { m_minLevel = min; m_maxLevel = max; }
  void SetNPCValue(FCULONG value)     { m_npcValue = value; }

  FCULONG GetID()                     { return m_id; }
  std::string GetName()               { return m_name; }
  FCULONG GetTypeID()                 { return m_typeID; }
#ifndef _FCCLIENT_COMPILE_
  FCULONG GetObjectID()               { return m_objID; }
#endif//_FCCLIENT_COMPILE_
  FCSHORT GetMinLevel()               { return m_minLevel; }
  FCSHORT GetMaxLevel()               { return m_maxLevel; }
  FCULONG GetNPCValue()               { return m_npcValue; }

private:

  FCULONG       m_id;
  std::string   m_name;
  FCULONG       m_typeID;
#ifndef _FCCLIENT_COMPILE_
  FCULONG       m_objID;
#endif//_FCCLIENT_COMPILE_
  FCSHORT       m_minLevel;
  FCSHORT       m_maxLevel;
  FCULONG       m_npcValue;
};

#endif//_ITEM_H_