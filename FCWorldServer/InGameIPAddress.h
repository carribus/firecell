#ifndef _INGAMEIPADDRESS_H_
#define _INGAMEIPADDRESS_H_

#include "../common/fctypes.h"

class InGameIPAddress
{
public:
  InGameIPAddress(void);
  ~InGameIPAddress(void);

private:

  FCBYTE  m_bAClass;
  FCBYTE  m_bBClass;
  FCBYTE  m_bCClass;
  FCBYTE  m_bDClass;
};

#endif//_INGAMEIPADDRESS_H_