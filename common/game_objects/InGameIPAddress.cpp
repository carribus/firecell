#include <string>
#include "InGameIPAddress.h"

InGameIPAddress::InGameIPAddress(void)
{
}

///////////////////////////////////////////////////////////////////////

InGameIPAddress::~InGameIPAddress(void)
{
}

///////////////////////////////////////////////////////////////////////

void InGameIPAddress::SetIP(const std::string& ipAddress)
{
  size_t offset = 0, offset2;
  std::string ipGroup;
  FCSHORT* groups[] = { &m_bAClass, &m_bBClass, &m_bCClass, &m_bDClass };
  size_t idx = 0;

  offset2 = ipAddress.find(".", offset);
  while ( offset2 != std::string::npos )
  {
    ipGroup = ipAddress.substr(offset, offset2-offset);
    *groups[idx] = (FCSHORT)atoi(ipGroup.c_str());
    idx++;
    offset = offset2+1;

    offset2 = ipAddress.find(".", offset);
    if ( offset2 == std::string::npos )
    {
      ipGroup = ipAddress.substr(offset, ipAddress.length());
      *groups[idx] = (FCSHORT)atoi(ipGroup.c_str());
    }
  }
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