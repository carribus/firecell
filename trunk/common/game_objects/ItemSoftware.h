#ifndef _ITEMSOFTWARE_H_
#define _ITEMSOFTWARE_H_

#include "Item.h"

class ItemSoftware : public Item
{
public:
  ItemSoftware(void);
  ~ItemSoftware(void);

  void SetSoftwareType(FCULONG type)                        { m_swType = type; }
  void IsService(bool bFlag)                                { m_bIsService = bFlag; }

  FCULONG GetSoftwareType()                                 { return m_swType; }
  bool IsService()                                          { return m_bIsService; }

protected:

  FCULONG m_swType;                 //!> Software Type
  bool m_bIsService;                //!> Flag indicating whether software is a service or a tool
  FCULONG m_scriptID;               //!> Temporary ID holder until we have a script object
};

#endif//_ITEMSOFTWARE_H_