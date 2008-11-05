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
#include "IEventSystem.h"
#include "InGameIPAddress.h"

using namespace std;

class Player : public IEventSource
{
public:

  DECLARE_EVENT_SOURCE();
  DECLARE_EVENT(LoggedIn);

  Player(void);
  Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip);
  ~Player(void);

  /*
   *  IEventSource implementation
   */
  const string& GetType()                 { return Player::EVTSYS_ObjectType; }

  /*
   *  Public Methods
   */

  FCULONG GetAccountID()                  { return m_accountID; }
  FCULONG GetID()                         { return m_id; }
  string GetName()                        { return m_name; }
  string GetEmail()                       { return m_email; }
  FCULONG GetXP()                         { return m_xp; }
  FCULONG GetLevel()                      { return m_level; }
  FCINT GetFameScale()                    { return m_fameScale; }
  FCULONG GetCityID()                     { return m_cityID; }
  FCULONG GetCountryID()                  { return m_countryID; }
  InGameIPAddress& GetIP()                { return m_ip; }
  FCSOCKET GetClientSocket()              { return m_clientSocket; }

  void SetAccountID(FCULONG id)           { m_accountID = id; }
  void SetID(FCULONG id)                  { m_id = id; }
  void SetName(string name)               { m_name = name; }
  void SetEmail(string email)             { m_email = email; }
  void SetXP(FCULONG xp)                  { m_xp = xp; }
  void SetLevel(FCULONG level)            { m_level = level; }
  void SetFameScale(FCINT scale)          { m_fameScale = scale; }
  void SetCityID(FCULONG id)              { m_cityID = id; }
  void SetCountryID(FCULONG id)           { m_countryID = id; }
  void SetIP(InGameIPAddress& ip)         { m_ip = ip; }
  void SetClientSocket(FCSOCKET sock)     { m_clientSocket = sock; }

private:

  FCULONG m_accountID;
  FCULONG m_id;
  string m_name;
  string m_email;
  FCULONG m_xp;
  FCULONG m_level;
  FCINT m_fameScale;
  FCULONG m_cityID;
  FCULONG m_countryID;
  InGameIPAddress m_ip;
  FCSOCKET m_clientSocket;
};

#endif//_PLAYER_H_