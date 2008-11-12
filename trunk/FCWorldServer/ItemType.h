#ifndef _ITEMTYPE_H_
#define _ITEMTYPE_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class ItemType
{
public:

  enum
  {
    Unknown,
    Processor,
    OS,
    Memory,
    Network,
    Storage,
    Software,
    Data,
    Miscellaneous,
    //------------
    LastType
  } Type;

  ItemType(void) : m_id(0)
  {
  }

  ~ItemType(void)
  {
  }

  void SetID(FCULONG id)                      { m_id = id; }
  void SetName(string name)                   { m_name = name; }
  void SetDBTable(string table)               { m_dbTable = table; }
  FCULONG GetID()                             { return m_id; }
  string GetName()                            { return m_name; }
  string GetDBTable()                         { return m_dbTable; }

private:

  FCULONG   m_id;
  string    m_name;
  string    m_dbTable;
};

#endif//_ITEMTYPE_H_