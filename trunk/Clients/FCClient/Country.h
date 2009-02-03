#ifndef _COUNTRY_H_
#define _COUNTRY_H_

#include <string>
#include <map>
#include "City.h"

class Country
{
public:

  Country(void)
    : m_id(0)
  {
  }

  ~Country(void)
  {
  }

  City GetCityByID(FCULONG id)
  {
    City& c = m_cities[id];

    return c;
  }

  void AddCity(City& city)
  {
    m_cities[city.GetID()] = city;
  }

  FCULONG GetID()                         { return m_id; }
  void SetID(FCULONG id)                  { m_id = id; }
  std::string GetName()                   { return m_name; }
  void SetName(const std::string& name)   { m_name = name; }
  size_t GetCityCount()                   { return m_cities.size(); }
	std::map<FCULONG, City>& GetCities()		{ return m_cities; }

private:

  FCULONG m_id;
  std::string m_name;
  std::map<FCULONG, City> m_cities;
};

#endif//_COUNTRY_H_