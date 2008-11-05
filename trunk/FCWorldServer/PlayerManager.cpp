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

///////////////////////////////////////////////////////////////////////

PlayerManager::PlayerManager(void)
{
  pthread_mutex_init(&m_mutexAliases, NULL);
  pthread_mutex_init(&m_mutexIDs, NULL);
}

///////////////////////////////////////////////////////////////////////

PlayerManager::~PlayerManager(void)
{
  RemoveAllPlayers();

  pthread_mutex_destroy(&m_mutexAliases);
  pthread_mutex_destroy(&m_mutexIDs);
}

///////////////////////////////////////////////////////////////////////

Player* PlayerManager::CreatePlayer(FCULONG accountID, FCULONG id, string name, FCULONG xp, FCULONG level, FCINT fame_scale, FCULONG country_id, FCULONG city_id)
{
  Player* pPlayer = NULL;

  // check if the player exists already
  if ( !(pPlayer = GetPlayerByName(name)) && !(pPlayer = GetPlayerByID(id)) )
  {
    // create the new player object
    pPlayer = new Player(accountID,               // account ID of the character
                         id,                      // character ID
                         name,                    // character name
                         "",                      // character's email address
                         xp,                      // character's xp
                         level,                   // character's level
                         fame_scale,              // character's famescale
                         country_id,              // character's country
                         city_id,                 // character's city
                         NULL);                   // character's in game IP address

    // add it to the maps
    pthread_mutex_lock(&m_mutexAliases);
    pthread_mutex_lock(&m_mutexIDs);

    m_mapAliases[name] = pPlayer;
    m_mapIDs[id] = pPlayer;

    pthread_mutex_unlock(&m_mutexAliases);
    pthread_mutex_unlock(&m_mutexIDs);
  }
  else
  {
    // player already exists in the manager... just pass this object out
  }

  return pPlayer;
}

///////////////////////////////////////////////////////////////////////

Player* PlayerManager::GetPlayerByName(string name)
{
  PlayerAliasMap::iterator it = m_mapAliases.find(name);

  if ( it != m_mapAliases.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

Player* PlayerManager::GetPlayerByID(FCULONG id)
{
  PlayerIDMap::iterator it = m_mapIDs.find(id);

  if ( it != m_mapIDs.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

void PlayerManager::RemoveAllPlayers()
{
  pthread_mutex_lock(&m_mutexAliases);
  m_mapAliases.erase( m_mapAliases.begin(), m_mapAliases.end() );
  pthread_mutex_unlock(&m_mutexAliases);
}