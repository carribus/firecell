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

#include <map>
#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "../common/PThreadRWLock.h"
#include "../Clients/common/Socket/ClientSocket.h"
#include "IEventSystem.h"
#include "Computer.h"
#include "Console.h"
#include "Mission.h"
#include "../common/game_objects/InGameIPAddress.h"
#include "../common/game_objects/PlayerBase.h"

using namespace std;

class Player : public PlayerBase
             , public IEventSource
             , public IEventTarget
{
public:

  DECLARE_EVENT_SOURCE();
  DECLARE_EVENT(LoggedIn);
  DECLARE_EVENT(LoggedOut);

  /*
   * Structure(s)
   */
	struct PlayerItem
	{
		FCULONG itemID;
		FCSHORT count;
	};

  Player(void);
  Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip = NULL);
  ~Player(void);

  /*
   *  IEventSource implementation
   */
  const string& GetType()                 { return Player::EVTSYS_ObjectType; }

  /*
   *  IEventTarget implementation
   */
  void RegisterForEvents(IEventSystem* pEventSystem);
  void OnEvent(IEventSource* pSource, IEvent* pEvent);

  /*
   *  Public Methods
   */
  Player* CreateSafeHandle();
	void AddXP(FCSHORT xpGain);

	/*
	 *	Mission related
	 */
	Mission* AcceptMission(Mission* pMission);
	bool HasMission(FCULONG missionID);
  Mission* GetMission(FCULONG missionID);
	bool HasCompletedMission(FCULONG missionID);
  FCULONG GetMissionCount(bool bActiveOnly = false);
  std::map<FCULONG, Mission*>& GetMissions()                        { return m_mapMissions; }

	/*
	 *	Item related
	 */
	void AddItem(FCULONG itemID, FCSHORT count = 1, bool bPersist = true);
	bool HasItem(FCULONG itemID);
	void RemoveItem(FCULONG itemID);
  size_t GetUniqueItemCount();
  const std::map<FCULONG, PlayerItem>& GetItems()                   { return m_mapItems; }
  void LockItemsForRead()                                           { m_itemLock.LockForRead(); }
  void LockItemsForWrite()                                          { m_itemLock.LockForWrite(); }
  void UnlockItems()                                                { m_itemLock.Unlock(); }
  void LockMissionsForRead()                                        { m_missionLock.LockForRead(); }
  void LockMissionsForWrite()                                       { m_missionLock.LockForWrite(); }
  void UnlockMissions()                                             { m_missionLock.Unlock(); }

  /*
	 *	Accessor/Mutators
	 */
  Computer& GetComputer()                 { return *m_computer; }
  Console& GetConsole()                   { return *m_console; }
  InGameIPAddress& GetIP()                { return m_ip; }

  void SetIP(InGameIPAddress& ip)         { m_ip = ip; }

private:

  /*
   *  Private Methods
   */
  void createComputer();
  void InsertCharacterItemInfo(FCULONG itemID, FCSHORT count);
  void UpdateCharacterItemInfo(FCULONG itemID, FCSHORT count);
  void DeleteCharacterItemInfo(FCULONG itemID);

  /*
   *  Private Members
   */
  Computer* m_computer;
  Console* m_console;
  InGameIPAddress m_ip;

  IEventSystem* m_pEventSystem;

	/*
	 *	Missions
	 */
	typedef std::map<FCULONG, Mission*> MissionMap;
	MissionMap m_mapMissions;
  PThreadRWLock m_missionLock;

	/*
	 *	Items
	 */
	typedef std::map<FCULONG, PlayerItem>		ItemMap;
	ItemMap	m_mapItems;
  PThreadRWLock m_itemLock;
};

#endif//_PLAYER_H_