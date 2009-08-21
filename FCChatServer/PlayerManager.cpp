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
#include "../common/threading.h"
#include "PlayerManager.h"

PlayerManager* PlayerManager::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

PlayerManager::PlayerManager()
{
}

///////////////////////////////////////////////////////////////////////

PlayerManager::~PlayerManager(void)
{
}

///////////////////////////////////////////////////////////////////////

PlayerManager& PlayerManager::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new PlayerManager;
  }
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void PlayerManager::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

Player* PlayerManager::CreatePlayer(FCULONG accountID, FCULONG id, string name, FCULONG xp, FCULONG level, FCINT fame_scale, FCULONG country_id, FCULONG city_id, FCSOCKET clientSocket)
{

  Player* pPlayer = _super::CreatePlayer(accountID, id, name, xp, level, fame_scale, country_id, city_id, clientSocket);

  if ( pPlayer )
  {
    // player created
  }
  else
  {
    // failed to create/load the player object... problems!!!
  }

  return pPlayer;
}
