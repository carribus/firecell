#ifndef _ITEMPROCESSOR_H_
#define _ITEMPROCESSOR_H_

#include "Item.h"

class ItemProcessor : public Item
{
public:
  ItemProcessor(void);
  ~ItemProcessor(void);

  void SetCoreCount(FCSHORT count)               { m_coreCount = count; }
  void SetCoreSpeed(FCSHORT speed)               { m_coreSpeed = speed; }
  FCSHORT GetCoreCount()                         { return m_coreCount; }
  FCSHORT GetCoreSpeed()                         { return m_coreSpeed; }

private:

  FCSHORT           m_coreCount;
  FCSHORT           m_coreSpeed;
};

#endif//_ITEMPROCESSOR_H_