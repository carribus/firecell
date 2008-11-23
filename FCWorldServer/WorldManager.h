#ifndef _WORLDMANAGER_H_
#define _WORLDMANAGER_H_

#include <map>
#include "Country.h"
#include "City.h"
#include "Company.h"
#include "Player.h"
#include "InGameIPAddress.h"

using namespace std;

class WorldManager
{
public:
  WorldManager(void);
  ~WorldManager(void);

  Country* AddCountry(FCULONG id, const string& name, FCSHORT IPGroupA);
  City* AddCity(Country* pCountry, FCULONG id, const string& name, FCSHORT IPGroupB);

  Country* GetCountry(FCULONG id);
  City* GetCity(FCULONG id);

  Company* AddCompany(FCULONG id, const string& name, FCULONG cityID, InGameIPAddress& ip);
  Company* GetCompany(FCULONG id);
  Company* GetCompanyByIP(InGameIPAddress& ip);

  bool GenerateIPAddress(const FCULONG countryID, const FCULONG cityID, InGameIPAddress& dest);

private:

  typedef map<FCULONG, Country*> CountryMap;
  CountryMap m_mapCountries;

  typedef map<FCULONG, City*> CityMap;
  CityMap m_mapCities;

  typedef map<FCULONG, Company*> CompanyMap;
  CompanyMap m_mapCompanies;
  CompanyMap m_mapIPToCompany;
};

#endif//_WORLDMANAGER_H_