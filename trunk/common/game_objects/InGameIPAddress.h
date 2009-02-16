#ifndef _INGAMEIPADDRESS_H_
#define _INGAMEIPADDRESS_H_

#include "../fctypes.h"

class InGameIPAddress
{
public:
  InGameIPAddress(void);
  ~InGameIPAddress(void);

  void SetIP(FCSHORT a, FCSHORT b, FCSHORT c, FCSHORT d);
  void GetIP(FCSHORT& a, FCSHORT& b, FCSHORT& c, FCSHORT& d);
  FCULONG ToULong();

  FCSHORT GetClassA()         { return m_bAClass; }
  FCSHORT GetClassB()         { return m_bBClass; }
  FCSHORT GetClassC()         { return m_bCClass; }
  FCSHORT GetClassD()         { return m_bDClass; }

  void SetClassA(FCSHORT a)   { m_bAClass = a; }
  void SetClassB(FCSHORT b)   { m_bBClass = b; }
  void SetClassC(FCSHORT c)   { m_bCClass = c; }
  void SetClassD(FCSHORT d)   { m_bDClass = d; }

private:

  FCSHORT  m_bAClass;
  FCSHORT  m_bBClass;
  FCSHORT  m_bCClass;
  FCSHORT  m_bDClass;
};

#endif//_INGAMEIPADDRESS_H_