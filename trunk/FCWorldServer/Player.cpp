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
#include "../common/fcqueries.h"
#include "../common/database/FCDatabase.h"
#include "../common/Logging/DynLog.h"
#include "../common/game_objects/FCObjectFactory.h"
#include "world_comms.h"
#include "Player.h"

DEFINE_EVENT_SOURCE(Player);
DEFINE_EVENT(Player, LoggedIn);
DEFINE_EVENT(Player, LoggedOut);

Player::Player(void)
: PlayerBase(Player::EVTSYS_ObjectType)
, m_pEventSystem(NULL)
{
  createComputer();
}

///////////////////////////////////////////////////////////////////////

Player::Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip)
: PlayerBase(accountID, id, name, email, xp, level, fameScale, cityID, countryID, EVTSYS_ObjectType)
, m_pEventSystem(NULL)
{
  createComputer();

  if ( ip )
    m_ip = *ip;
}

///////////////////////////////////////////////////////////////////////

Player::~Player(void)
{
  if ( m_pEventSystem )
  {
    m_pEventSystem->UnregisterEventTarget(this, Player::EVT_LoggedIn);
    m_pEventSystem->UnregisterEventTarget(this, Player::EVT_LoggedOut);
  }

  if ( m_console )
    delete m_console;
}

///////////////////////////////////////////////////////////////////////

void Player::RegisterForEvents(IEventSystem* pEventSystem)
{
  if ( !pEventSystem )
    return;

  m_pEventSystem = pEventSystem;

  // test listener
  pEventSystem->RegisterEventTarget(this, Player::EVT_LoggedIn);
  pEventSystem->RegisterEventTarget(this, Player::EVT_LoggedOut);
}

///////////////////////////////////////////////////////////////////////

void Player::OnEvent(IEventSource* pSource, IEvent* pEvent)
{
  string sourceType = (pSource ? pSource->GetType() : "World");
  string eventCode = pEvent->GetCode();
  string eventName = eventCode.substr(eventCode.find('.')+1, eventCode.length());

  DYNLOG_ADDLOG( DYNLOG_FORMAT("Player::OnEvent(): Event [%s] received from source [%s]", eventCode.c_str(), sourceType.c_str()) );

	// check for a Player event
  if ( !sourceType.compare( Player::EVTSYS_ObjectType ) )      // player events
  {
    Player* pPlayer = (Player*)pEvent->GetParam();

    // ensure that we do not handle our own event
    if ( pSource != (IEventSource*)this )
    {
      // handle events
      if ( !eventCode.compare( Player::EVT_LoggedIn ) )
      {
      }
    }
  }
	// check for a mission event
	else if ( !sourceType.compare( Mission::EVTSYS_ObjectType ) )
  {
		Mission* pMission = static_cast<Mission*>(pSource);
    Player* pPlayer = (Player*)pEvent->GetPlayer();
		if ( !eventCode.compare( Mission::EVT_Complete ) )
		{
			DYNLOG_ADDLOG( DYNLOG_FORMAT("Player %s completed mission id %ld (earned %ld XP)", GetName().c_str(), (FCULONG)(pEvent->GetParam()), pMission->GetSuccessXP()) );
			// need to send a message to the client here
      SendMissionComplete((FCULONG) pEvent->GetParam(), pPlayer->GetRouterSocket(), pPlayer->GetClientSocket());
			// Add the xp to the player
			pPlayer->AddXP( pMission->GetSuccessXP() );
		}
	}
  else
  {
    // its not a player event, so it may be related to a mission or something else that happened to us
    bool bHandled = false;

		// check if we have any missions
		if ( m_mapMissions.size() )
		{
			// hand the event off to all the missions we have...
			MissionMap::iterator it = m_mapMissions.begin();
			MissionMap::iterator limit = m_mapMissions.end();

			for ( ; it != limit; it++ )
			{
				it->second->OnEvent(pSource, pEvent);
			}
		}

    // if we didn't handle the event, then log the incident
    if ( !bHandled )
      DYNLOG_ADDLOG( DYNLOG_FORMAT("Player::OnEvent(): Unknown event received [source:%s / event:%s]", sourceType.c_str(), pEvent->GetCode().c_str()) );
  }
}

///////////////////////////////////////////////////////////////////////

Player* Player::CreateSafeHandle()
{
  Player* p = new Player;
  p->m_accountID = m_accountID;
  p->m_cityID = m_cityID;
  p->m_clientSocket = m_clientSocket;
  p->m_countryID = m_countryID;
  p->m_email = m_email;
  p->m_fameScale = m_fameScale;
  p->m_id = m_id;
  p->m_ip = m_ip;
  p->m_level = m_level;
  p->m_name = m_name;
  p->m_xp = m_xp;

  return p;
}

///////////////////////////////////////////////////////////////////////

void Player::AddXP(FCSHORT xpGain)
{
	m_xp += xpGain;
	SendXPGained(xpGain, m_xp, GetRouterSocket(), GetClientSocket());
}

///////////////////////////////////////////////////////////////////////

Mission* Player::AcceptMission(Mission* pMission)
{
	if ( !pMission )
		return NULL;

	Mission* pMyMission = this->GetMission(pMission->GetID());;
  
  if ( !pMyMission )
  {
    pMyMission = pMission->Clone();
	
	  if ( pMyMission )
	  {
      m_missionLock.LockForWrite();
		  m_mapMissions[ pMyMission->GetID() ] = pMyMission;
		  pMyMission->SetComplete(false);
      if ( pMyMission->GetParentID() != 0 )
      {
        MissionMap::iterator it = m_mapMissions.find( pMyMission->GetParentID() );

        if ( it != m_mapMissions.end() )
        {
          it->second->AddChildMission(pMyMission);
        }
        else
        {
          DYNLOG_ADDLOG( DYNLOG_FORMAT("Player::AcceptMission(): Could not add mission id %ld as a child to mission %ld because parent does not exist in Player %ld object", pMyMission->GetID(), pMyMission->GetParentID(), m_id) );
        }
      }
      m_missionLock.Unlock();
	  }
  }

  return pMyMission;
}

///////////////////////////////////////////////////////////////////////

Mission* Player::GetMission(FCULONG missionID)
{
  m_missionLock.LockForRead();
  MissionMap::iterator it = m_mapMissions.find(missionID);
  Mission* pMission = NULL;

  if ( it != m_mapMissions.end() )
  {
    pMission = it->second;
  }

  m_missionLock.Unlock();

  return pMission;
}

///////////////////////////////////////////////////////////////////////

bool Player::HasMission(FCULONG missionID)
{
  bool bResult = false;

  m_missionLock.LockForRead();
  MissionMap::iterator it = m_mapMissions.find(missionID);
  bResult = (it != m_mapMissions.end());
  m_missionLock.Unlock();

	return bResult;
}

///////////////////////////////////////////////////////////////////////

bool Player::HasCompletedMission(FCULONG missionID)
{
	bool bResult = false;

  m_missionLock.LockForRead();
  MissionMap::iterator it = m_mapMissions.find(missionID);
  if ( it != m_mapMissions.end() )
    bResult = it->second->IsComplete();
  m_missionLock.Unlock();

	return bResult;
}

///////////////////////////////////////////////////////////////////////

FCULONG Player::GetMissionCount(bool bActiveOnly)
{
  m_missionLock.LockForRead();
  FCULONG count = 0;
  
  if ( !bActiveOnly )
  {
    count = (FCULONG)m_mapMissions.size();
  }
  else
  {
    MissionMap::iterator it = m_mapMissions.begin();
    MissionMap::iterator limit = m_mapMissions.end();

    for ( ; it != limit; ++it )
    {
      if ( !it->second->IsComplete() )
        count++;
    }
  }
  m_missionLock.Unlock();

  return count;
}

///////////////////////////////////////////////////////////////////////

void Player::AddItem(FCULONG itemID, FCSHORT count, bool bPersist)
{
  m_itemLock.LockForWrite();

	ItemMap::iterator it = m_mapItems.find(itemID);

	if ( it == m_mapItems.end() )
	{
		// add a new item to the map
		PlayerItem gi = {itemID, count};
		m_mapItems[itemID] = gi;
    if ( bPersist )
      InsertCharacterItemInfo(itemID, count);
	}
	else
	{
		it->second.count++;
    if ( bPersist )
      UpdateCharacterItemInfo(itemID, it->second.count);

	}

  m_itemLock.Unlock();
}

///////////////////////////////////////////////////////////////////////

bool Player::HasItem(FCULONG itemID)
{
  bool bResult = false;

  m_itemLock.LockForRead();
	ItemMap::iterator it = m_mapItems.find(itemID);
  m_itemLock.Unlock();

  if ( it != m_mapItems.end() )
    bResult = (it->second.count > 0);

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void Player::RemoveItem(FCULONG itemID)
{
  m_itemLock.LockForWrite();
	ItemMap::iterator it = m_mapItems.find(itemID);

	if ( it != m_mapItems.end() )
  {
		it->second.count--;
    if ( it->second.count > 0 )
    {
      UpdateCharacterItemInfo(itemID, it->second.count);
    }
    else
    {
      m_mapItems.erase(it);
      DeleteCharacterItemInfo(itemID);
    }
  }
  m_itemLock.Unlock();
}

///////////////////////////////////////////////////////////////////////

size_t Player::GetUniqueItemCount()
{
  return m_mapItems.size();
}

///////////////////////////////////////////////////////////////////////

void Player::createComputer()
{
  m_computer = FCObjectFactory::instance().createObject<Computer>(this);
  m_console = new Console(*this);
}

///////////////////////////////////////////////////////////////////////

void Player::InsertCharacterItemInfo(FCULONG itemID, FCSHORT count)
{
  FCDatabase& db = FCDatabase::instance();

  db.ExecuteJob(DBQ_INSERT_CHARACTER_ITEM_INFO, NULL, m_id, itemID, count);
}

///////////////////////////////////////////////////////////////////////

void Player::UpdateCharacterItemInfo(FCULONG itemID, FCSHORT count)
{
  FCDatabase& db = FCDatabase::instance();

  db.ExecuteJob(DBQ_UPDATE_CHARACTER_ITEM_INFO, NULL, count, m_id, itemID);
}

///////////////////////////////////////////////////////////////////////

void Player::DeleteCharacterItemInfo(FCULONG itemID)
{
  FCDatabase& db = FCDatabase::instance();

  db.ExecuteJob(DBQ_DELETE_CHARACTER_ITEM_INFO, NULL, m_id, itemID);
}
