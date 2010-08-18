#ifndef _PLAYERMANAGERBASE_H_
#define _PLAYERMANAGERBASE_H_

#include <map>
#include "../common/threading.h"
#include "../fctypes.h"

class PlayerBase;

template <class PlayerObj = PlayerBase>
class PlayerManagerBase
{
public:

  PlayerManagerBase(void)
  {
    pthread_mutex_init(&m_mutexAliases, NULL);
    pthread_mutex_init(&m_mutexIDs, NULL);
    pthread_mutex_init(&m_mutexClientSocks, NULL);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  virtual ~PlayerManagerBase(void)
  {
    RemoveAllPlayers();

    pthread_mutex_destroy(&m_mutexAliases);
    pthread_mutex_destroy(&m_mutexIDs);
    pthread_mutex_destroy(&m_mutexClientSocks);
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  PlayerObj* CreatePlayer(FCULONG accountID, FCULONG id, std::string name, FCULONG xp, FCULONG level, FCINT fame_scale, FCULONG country_id, FCULONG city_id, FCSOCKET clientSocket)
  {
    PlayerObj* pPlayer = NULL;

    // check if the player exists already
    if ( !(pPlayer = GetPlayerByName(name)) && !(pPlayer = GetPlayerByID(id)) )
    {
      // create the new player object
      pPlayer = new PlayerObj(accountID,               // account ID of the character
                           id,                      // character ID
                           name,                    // character name
                           "",                      // character's email address
                           xp,                      // character's xp
                           level,                   // character's level
                           fame_scale,              // character's famescale
                           country_id,              // character's country
                           city_id);                // character's city

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
    }
    else
    {
      // player already exists in the manager... just pass this object out
/*
      // update the socket mapping 
      m_mapClientSockets.erase( pPlayer->GetClientSocket() );
      m_mapClientSockets[clientSocket] = pPlayer;
      // update the client socket in the player object
      pPlayer->SetClientSocket(clientSocket);
*/
    }

    return pPlayer;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  PlayerObj* GetPlayerByName(std::string name)
  {
    PlayerAliasMap::iterator it = m_mapAliases.find(name);

    if ( it != m_mapAliases.end() )
      return it->second;

    return NULL;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  PlayerObj* GetPlayerByID(FCULONG id)
  {
    PlayerIDMap::iterator it = m_mapIDs.find(id);

    if ( it != m_mapIDs.end() )
      return it->second;

    return NULL;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  PlayerObj* GetPlayerByClientSocket(FCSOCKET s)
  {
    PlayerSocketMap::iterator it = m_mapClientSockets.find(s);

    if ( it != m_mapClientSockets.end() )
      return it->second;

    return NULL;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////

  void RemovePlayer(PlayerObj*& pPlayer)
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

  //////////////////////////////////////////////////////////////////////////////////////////////////

  void RemoveAllPlayers()
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

protected:

  /*
   *  Map of players by their alias
   */
  typedef std::map<std::string, PlayerObj*> PlayerAliasMap;
  PlayerAliasMap m_mapAliases;
  pthread_mutex_t m_mutexAliases;

  /*
   *  Map of players by their ID
   */
  typedef std::map<FCULONG, PlayerObj*> PlayerIDMap;
  PlayerIDMap m_mapIDs;
  pthread_mutex_t m_mutexIDs;

  /*
   *  Map of players by their client socket
   */ 
  typedef std::map<FCSOCKET, PlayerObj*> PlayerSocketMap;
  PlayerSocketMap m_mapClientSockets;
  pthread_mutex_t m_mutexClientSocks;

};

#endif//_PLAYERMANAGERBASE_H_