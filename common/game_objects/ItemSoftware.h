#ifndef _ITEMSOFTWARE_H_
#define _ITEMSOFTWARE_H_

#include "Item.h"

class ItemSoftware : public Item
{
public:
  ItemSoftware(void);
  ~ItemSoftware(void);

  void SetSoftwareType(FCSHORT type)                        { m_swType = type; }
  void IsService(bool bFlag)                                { m_bIsService = bFlag; }
  void SetCPUCost(FCSHORT cost)                             { m_cpuCost = cost; }
  void SetMemCost(FCULONG cost)                             { m_memCost = cost; }

  FCSHORT GetSoftwareType()                                 { return m_swType; }
  bool IsService()                                          { return m_bIsService; }
  FCSHORT GetCPUCost()                                      { return m_cpuCost; }
  FCULONG GetMemCost()                                      { return m_memCost; }

protected:

  FCSHORT m_swType;                 //!> Software Type
  bool m_bIsService;                //!> Flag indicating whether software is a service or a tool
  FCULONG m_scriptID;               //!> Temporary ID holder until we have a script object
  FCSHORT m_cpuCost;                //!> CPU Cost of the software
  FCULONG m_memCost;                //!> Memory cost of the software
};

#endif//_ITEMSOFTWARE_H_