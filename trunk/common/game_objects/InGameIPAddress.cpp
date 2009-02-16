#include "InGameIPAddress.h"

InGameIPAddress::InGameIPAddress(void)
{
}

///////////////////////////////////////////////////////////////////////

InGameIPAddress::~InGameIPAddress(void)
{
}

///////////////////////////////////////////////////////////////////////

void InGameIPAddress::SetIP(FCSHORT a, FCSHORT b, FCSHORT c, FCSHORT d)
{
  m_bAClass = a;
  m_bBClass = b;
  m_bCClass = c;
  m_bDClass = d;
}

///////////////////////////////////////////////////////////////////////

void InGameIPAddress::GetIP(FCSHORT& a, FCSHORT& b, FCSHORT& c, FCSHORT& d)
{
  a = m_bAClass;
  b = m_bBClass;
  c = m_bCClass;
  d = m_bDClass;
}

///////////////////////////////////////////////////////////////////////

FCULONG InGameIPAddress::ToULong()
{
  FCULONG ip = 0;

  ip = (m_bAClass & 0x00FF) << 24 |
       (m_bBClass & 0x00FF) << 16 |
       (m_bCClass & 0x00FF) << 8 |
       (m_bDClass & 0x00FF);

  return ip;
}