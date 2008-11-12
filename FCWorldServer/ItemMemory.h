#ifndef _ITEMMEMORY_H_
#define _ITEMMEMORY_H_
#include "Item.h"

class ItemMemory : public Item
{
public:
  ItemMemory(void);
  ~ItemMemory(void);

  void SetMemorySize(FCULONG size)                { m_memorySize = size; }
  FCULONG GetMemorySize()                         { return m_memorySize; }

private:

  FCULONG           m_memorySize;
};

#endif//_ITEMMEMORY_H_