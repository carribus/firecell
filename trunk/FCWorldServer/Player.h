/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "../common/fctypes.h"
#include "InGameIPAddress.h"

using namespace std;

class Player
{
public:
  Player(void);
  ~Player(void);

  FCULONG GetID()                         { return m_id; }
  string GetName()                        { return m_name; }
  string GetEmail()                       { return m_email; }
  FCULONG GetXP()                         { return m_xp; }
  FCULONG GetLevel()                      { return m_level; }
  FCINT GetFameScale()                    { return m_fameScale; }
  FCULONG GetCityID()                     { return m_cityID; }
  FCULONG GetCountryID()                  { return m_countryID; }
  InGameIPAddress& GetIP()                { return m_ip; }

private:

  FCULONG m_id;
  string m_name;
  string m_email;
  FCULONG m_xp;
  FCULONG m_level;
  FCINT m_fameScale;
  FCULONG m_cityID;
  FCULONG m_countryID;
  InGameIPAddress m_ip;
};

#endif//_PLAYER_H_