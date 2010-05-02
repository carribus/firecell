#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include "../../../common/fctypes.h"
#include "../../../common/game_objects/ComputerBase.h"
#include "../../../common/game_objects/InGameIPAddress.h"

using namespace std;

class Character
{
public:
  Character(FCUINT id, const QString& name);
	Character(const Character& src);
  ~Character(void);

  void SetXP(FCUINT xp)                               { m_xp = xp; }
  void SetLevel(FCUINT level)                         { m_level = level; }
  void SetFameScale(FCUINT scale)                     { m_fame_scale = scale; }
  void SetCountryID(FCBYTE countryID)                 { m_country_id = countryID; }
  void SetCityID(FCBYTE cityID)                       { m_city_id = cityID; }
  
  FCUINT GetID()                                      { return m_id; }
  QString GetName()                                   { return m_name; }
  FCUINT GetXP()                                      { return m_xp; }
  FCUINT GetLevel()                                   { return m_level; }
  FCUINT GetFameScale()                               { return m_fame_scale; }
  FCBYTE GetCountrID()                                { return m_country_id; }
  FCBYTE GetCityID()                                  { return m_city_id; }
  InGameIPAddress& GetIP()                            { return m_ip; }
  ComputerBase& GetComputer()                         { return *m_computer; }

private:

  FCUINT     m_id;
  QString    m_name;
  FCUINT     m_xp;
  FCUINT     m_level;
  FCUINT     m_fame_scale;
  FCBYTE     m_country_id;
  FCBYTE     m_city_id;

  InGameIPAddress m_ip;
  ComputerBase* m_computer;
};

#endif//_CHARACTER_H_