#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include "../../common/fctypes.h"
#include "../../common/game_objects/ComputerBase.h"
#include "../../common/game_objects/InGameIPAddress.h"
#include <string>

using namespace std;

class Character
{
public:
  Character(FCUINT id, const wstring& name);
  ~Character(void);

  void SetXP(FCUINT xp)                               { m_xp = xp; }
  void SetLevel(FCUINT level)                         { m_level = level; }
  void SetFameScale(FCUINT scale)                     { m_fame_scale = scale; }
  void SetCountryID(FCBYTE countryID)                 { m_country_id = countryID; }
  void SetCityID(FCBYTE cityID)                       { m_city_id = cityID; }
  
  FCUINT GetID()                                      { return m_id; }
  wstring GetName()                                   { return m_name; }
  FCUINT GetXP()                                      { return m_xp; }
  FCUINT GetLevel()                                   { return m_level; }
  FCUINT GetFameScale()                               { return m_fame_scale; }
  FCBYTE GetCountrID()                                { return m_country_id; }
  FCBYTE GetCityID()                                  { return m_city_id; }
  InGameIPAddress& GetIP()                            { return m_ip; }
  ComputerBase& GetComputer()                         { return m_computer; }

private:

  FCUINT m_id;
  wstring m_name;
  FCUINT m_xp;
  FCUINT m_level;
  FCUINT m_fame_scale;
  FCBYTE m_country_id;
  FCBYTE m_city_id;

  InGameIPAddress m_ip;
  ComputerBase m_computer;
};

#endif//_CHARACTER_H_