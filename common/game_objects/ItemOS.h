#ifndef _ITEMOS_H_
#define _ITEMOS_H_

#include "Item.h"

class ItemOS : public Item
{
public:
  ItemOS(void);
  ~ItemOS(void);

  void SetKernelID(FCSHORT id)                        { m_kernelID = id; }
  void SetKernelName(std::string name)                { m_kernelName = name; }
  FCSHORT GetKernelID()                               { return m_kernelID; }
  std::string GetKernelName()                         { return m_kernelName; }

private:

  FCSHORT           m_kernelID;
  std::string       m_kernelName;
};

#endif//_ITEMOS_H_