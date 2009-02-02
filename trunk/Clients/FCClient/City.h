#ifndef _CITY_H_
#define _CITY_H_

#include <string>
#include <vector>
#include "../../common/fctypes.h"

class City
{
public:

  City(void)
    : m_id(0)
    , m_country_id(0)
  {
  }

  ~City(void)
  {
  }

  FCULONG GetID()                         { return m_id; }
  void SetID(FCULONG id)                  { m_id = id; }
  FCULONG GetCountryID()                  { return m_country_id; }
  void SetCountryID(FCULONG id)           { m_country_id = id; }
  std::string GetName()                   { return m_name; }
  void SetName(const std::string& name)   { m_name = name; }

private:

  FCULONG m_id;
  FCULONG m_country_id;
  std::string m_name;
};

#endif//_CITY_H_