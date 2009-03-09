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

PlayerManager::PlayerManager(IEventSystem* pEventSystem)
: m_pEventSystem(pEventSystem)
{
  pthread_mutex_init(&m_mutexAliases, NULL);
  pthread_mutex_init(&m_mutexIDs, NULL);
  pthread_mutex_init(&m_mutexClientSocks, NULL);
}

///////////////////////////////////////////////////////////////////////

PlayerManager::~PlayerManager(void)
{
  RemoveAllPlayers();

  pthread_mutex_destroy(&m_mutexAliases);
  pthread_mutex_destroy(&m_mutexIDs);
  pthread_mutex_destroy(&m_mutexClientSocks);
}

///////////////////////////////////////////////////////////////////////

Player* PlayerManager::CreatePlayer(FCULONG accountID, FCULONG id, string name, FCULONG xp, FCULONG level, FCINT fame_scale, FCULONG country_id, FCULONG city_id, FCSOCKET clientSocket)
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

    // configure the player's network ports (8 ports by default)
    for ( int i = 0; i < 8; i++ )
      pPlayer->GetComputer().GetNetworkPorts().AddPort();

    pPlayer->SetClientSocket(clientSocket);

    // add it to the maps
    pthread_mutex_lock(&m_mutexAliases);
    pthread_mutex_lock(&m_mutexIDs);
    pthread_mutex_lock(&m_mutexClientSocks);

    m_mapAliases[name] = pPlayer;
    m_mapIDs[id] = pPlayer;
    m_mapClientSockets[clientSocket] = pPlayer;

    pthread_mutex_unlock(&m_mutexAliases);
    pthread_mutex_unlock(&m_mutexIDs);
    pthread_mutex_unlock(&m_mutexClientSocks);

    // register the object with the eventing system
    pPlayer->RegisterForEvents(m_pEventSystem);

  }
  else
  {
    // player already exists in the manager... just pass this object out

    // update the socket mapping 
    m_mapClientSockets.erase( pPlayer->GetClientSocket() );
    m_mapClientSockets[clientSocket] = pPlayer;
    // update the client socket in the player object
    pPlayer->SetClientSocket(clientSocket);
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

Player* PlayerManager::GetPlayerByClientSocket(FCSOCKET s)
{
  PlayerSocketMap::iterator it = m_mapClientSockets.find(s);

  if ( it != m_mapClientSockets.end() )
    return it->second;

  return NULL;
}

///////////////////////////////////////////////////////////////////////

void PlayerManager::RemovePlayer(Player*& pPlayer)
{
  if ( !pPlayer )
    return;

  pthread_mutex_lock(&m_mutexAliases);
  pthread_mutex_lock(&m_mutexIDs);
  pthread_mutex_lock(&m_mutexClientSocks);

  m_mapAliases.erase( pPlayer->GetName() );
  m_mapIDs.erase( pPlayer->GetID() );
  m_mapClientSockets.erase( pPlayer->GetClientSocket() );

  delete pPlayer;
  pPlayer = NULL;

  pthread_mutex_unlock(&m_mutexClientSocks);
  pthread_mutex_unlock(&m_mutexIDs);
  pthread_mutex_unlock(&m_mutexAliases);
}

///////////////////////////////////////////////////////////////////////

void PlayerManager::RemoveAllPlayers()
{
  pthread_mutex_lock(&m_mutexAliases);
  pthread_mutex_lock(&m_mutexIDs);
  pthread_mutex_lock(&m_mutexClientSocks);

  m_mapAliases.erase( m_mapAliases.begin(), m_mapAliases.end() );
  m_mapIDs.erase( m_mapIDs.begin(), m_mapIDs.end() );

  PlayerSocketMap::iterator it;
  for ( it = m_mapClientSockets.begin(); it != m_mapClientSockets.end(); it++ )
  {
    delete it->second;
  }
  m_mapClientSockets.erase( m_mapClientSockets.begin(), m_mapClientSockets.end() );

  pthread_mutex_unlock(&m_mutexClientSocks);
  pthread_mutex_unlock(&m_mutexIDs);
  pthread_mutex_unlock(&m_mutexAliases);

}