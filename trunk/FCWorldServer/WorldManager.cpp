#include "WorldManager.h"

WorldManager::WorldManager(void)
{
}

///////////////////////////////////////////////////////////////////////

WorldManager::~WorldManager(void)
{
  // remove cities
  CountryMap::iterator it = m_mapCountries.begin();

  while ( it != m_mapCountries.end() )
  {
    delete it->second;
    it++;
  }
  m_mapCountries.erase( m_mapCountries.begin(), m_mapCountries.end() );

  // remove countries
  CityMap::iterator it2 = m_mapCities.begin();

  while ( it2 != m_mapCities.end() )
  {
    delete it2->second;
    it2++;
  }
  m_mapCities.erase( m_mapCities.begin(), m_mapCities.end() );

  // remove companies
  CompanyMap::iterator it3 = m_mapCompanies.begin();

  while ( it3 != m_mapCompanies.end() )
  {
    delete it3->second;
    it3++;
  }
  m_mapCompanies.erase( m_mapCompanies.begin(), m_mapCompanies.end() );
  m_mapIPToCompany.erase( m_mapIPToCompany.begin(), m_mapIPToCompany.end() );
}

///////////////////////////////////////////////////////////////////////

Country* WorldManager::AddCountry(FCULONG id, const string& name, FCSHORT IPGroupA)
{
  Country* pCountry = NULL;

  if ( !(pCountry = GetCountry(id)) )
  {
    pCountry = new Country;
    pCountry->SetID(id);
    pCountry->SetName(name);
    pCountry->SetIPGroupA(IPGroupA);

    m_mapCountries[id] = pCountry;
  }

  return pCountry;
}

///////////////////////////////////////////////////////////////////////

City* WorldManager::AddCity(Country* pCountry, FCULONG id, const string& name, FCSHORT IPGroupB)
{
  City* pCity = NULL;

  if ( pCountry && !(pCity = GetCity(id)) )
  {
    pCity = new City(pCountry);
    pCity->SetID(id);
    pCity->SetName(name);
    pCity->SetIPGroupB(IPGroupB);

    m_mapCities[id] = pCity;
  }

  return pCity;
}

///////////////////////////////////////////////////////////////////////

Country* WorldManager::GetCountry(FCULONG id)
{
  CountryMap::iterator it = m_mapCountries.find(id);

  if ( it != m_mapCountries.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

City* WorldManager::GetCity(FCULONG id)
{
  CityMap::iterator it = m_mapCities.find(id);

  if ( it != m_mapCities.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

Company* WorldManager::AddCompany(FCULONG id, const string& name, FCULONG cityID, InGameIPAddress& ip)
{
  Company* pCompany = NULL;

  if ( !(pCompany = GetCompany(id)) )
  {
    pCompany = new Company;
    pCompany->SetID(id);
    pCompany->SetName(name);
    pCompany->SetCityID(cityID);

    if ( ip.GetClassC() == 0 && ip.GetClassD() == 0 )
    {
      ip.SetClassC( rand() % 255 );
      ip.SetClassD( (rand() % 254)+1 );
    }

    pCompany->SetIP(ip);

    m_mapCompanies[id] = pCompany;
    m_mapIPToCompany[ ip.ToULong() ] = pCompany;
  }

  return pCompany;
}

///////////////////////////////////////////////////////////////////////

Company* WorldManager::GetCompany(FCULONG id)
{
  CompanyMap::iterator it = m_mapCompanies.find(id);

  if ( it != m_mapCompanies.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

Company* WorldManager::GetCompanyByIP(InGameIPAddress& ip)
{
  FCULONG ipAddy = ip.ToULong();
  CompanyMap::iterator it = m_mapIPToCompany.find(ipAddy);

  if ( it != m_mapIPToCompany.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

bool WorldManager::GenerateIPAddress(const FCULONG countryID, const FCULONG cityID, InGameIPAddress& dest)
{
  Country* pCountry = GetCountry(countryID);
  City* pCity = GetCity(cityID);

  if ( !pCountry || !pCity )
    return false;

  dest.SetIP( pCountry->GetIPGroupA(), pCity->GetIPGroupB(), rand() % 255, (rand() % 254)+1);

  return true;
}
