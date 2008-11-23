#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class Country
{
public:

  Country(void) : m_id(0), m_IPGroupA(0)
  {
  }

  ~Country(void)
  {
  }

  FCULONG GetID()                                 { return m_id; }
  string GetName()                                { return m_name; }
  FCSHORT GetIPGroupA()                           { return m_IPGroupA; }
  void SetID(FCULONG id)                          { m_id = id; }
  void SetName(const string& name)                { m_name = name; }
  void SetIPGroupA(FCSHORT ip)                    { m_IPGroupA = ip; }

private:

  FCULONG       m_id;
  string        m_name;
  FCSHORT       m_IPGroupA;
};

#endif//_COUNTRY_H_