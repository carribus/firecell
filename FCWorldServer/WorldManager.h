#ifndef _WORLDMANAGER_H_
#define _WORLDMANAGER_H_

#include <map>
#include "Country.h"
#include "City.h"
#include "Company.h"
#include "Player.h"
#include "../common/game_objects/InGameIPAddress.h"

using namespace std;

class WorldManager
{
public:

  struct NetConnection
  {
    enum ConnectionType
    {
      None = 0,
      Player = 1,
      NPC,
      Company
    };

    NetConnection() : connType(None), objID(0) {}

    InGameIPAddress ipAddress;
    ConnectionType connType;
    FCULONG objID;
  };

  WorldManager(void);
  ~WorldManager(void);

  /*
   *  Countries
   */
  Country* AddCountry(FCULONG id, const string& name, FCSHORT IPGroupA, bool bCharCreationAllowed);
  Country* GetCountry(FCULONG id);
  FCULONG GetAllCountries(std::vector<Country>& target, bool bSupportCharCreationOnly);

  /*
   *  Cities
   */
  City* AddCity(Country* pCountry, FCULONG id, const string& name, FCSHORT IPGroupB, bool bCharCreationAllowed);
  City* GetCity(FCULONG id);
  FCULONG GetAllCities(std::vector<City>& target, bool bSupportCharCreationOnly);

  /*
   *  Companies
   */
  Company* AddCompany(FCULONG id, const string& name, FCULONG cityID, InGameIPAddress& ip);
  Company* GetCompany(FCULONG id);
  Company* GetCompanyByIP(InGameIPAddress& ip);

  /*
   *  The Network
   */
  bool AddToNetwork(InGameIPAddress& ip, WorldManager::NetConnection::ConnectionType connType, FCULONG objID);
  bool GetNetworkObject(InGameIPAddress& ip, WorldManager::NetConnection& dest);

  bool GenerateIPAddress(const FCULONG countryID, const FCULONG cityID, InGameIPAddress& dest);

private:

  /*
   *  Countries
   */
  typedef map<FCULONG, Country*> CountryMap;
  CountryMap m_mapCountries;

  /*
   *  Cities
   */
  typedef map<FCULONG, City*> CityMap;
  CityMap m_mapCities;

  /*
   *  Companies
   */
  typedef map<FCULONG, Company*> CompanyMap;
  CompanyMap m_mapCompanies;
  CompanyMap m_mapIPToCompany;

  /*
   *  Network connections
   */
  typedef map<FCULONG, NetConnection> NetConnectionMap;
  NetConnectionMap m_mapNetConnections;
};

#endif//_WORLDMANAGER_H_