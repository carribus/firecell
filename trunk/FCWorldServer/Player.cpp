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
#include "Player.h"

DEFINE_EVENT_SOURCE(Player);
DEFINE_EVENT(Player, LoggedIn);

Player::Player(void)
: m_accountID(0)
, m_id(0)
, m_xp(0)
, m_level(0)
, m_fameScale(0)
, m_cityID(0)
, m_countryID(0)
, m_clientSocket(0)
{
}

///////////////////////////////////////////////////////////////////////

Player::Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip)
: m_accountID(accountID)
, m_id(id)
, m_name(name)
, m_email(email)
, m_xp(xp)
, m_level(level)
, m_fameScale(fameScale)
, m_cityID(cityID)
, m_countryID(countryID)
, m_clientSocket(0)
{
  if ( ip )
    m_ip = *ip;
}

///////////////////////////////////////////////////////////////////////

Player::~Player(void)
{
}
