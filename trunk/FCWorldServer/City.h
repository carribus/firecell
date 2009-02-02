#ifndef _CITY_H_
#define _CITY_H_

#include <string>
#include "../common/fctypes.h"
#include "Country.h"

using namespace std;

class City
{
public:

  City(Country* pCountry, bool CharCreationAllowed = true) : m_id(0), m_pCountry(pCountry), m_IPGroupB(0), m_bAllowCharCreationHere(CharCreationAllowed)
  {
  }

  ~City(void)
  {
  }

  FCULONG GetID()                         { return m_id; }
  string GetName()                        { return m_name; }
  Country* GetCountry()                   { return m_pCountry; }
  FCSHORT GetIPGroupB()                   { return m_IPGroupB; }
  bool IsCharCreationSupported()          { return m_bAllowCharCreationHere; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(const string& name)        { m_name = name; }
  void SetIPGroupB(FCSHORT ip)            { m_IPGroupB = ip; }
  void AllowCharCreation(bool bAllow)     { m_bAllowCharCreationHere = bAllow; }

private:

  FCULONG           m_id;
  string            m_name;
  Country*          m_pCountry;
  FCSHORT           m_IPGroupB;
  bool              m_bAllowCharCreationHere;
};

#endif//_CITY_H_