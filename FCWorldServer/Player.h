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
#include "Mission.h"
#include "../common/game_objects/FCObject.h"
#include "../common/game_objects/InGameIPAddress.h"

using namespace std;

class Player : public FCObject
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
  Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip);
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
	void AddItem(FCULONG itemID);
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
  FCULONG GetAccountID() const            { return m_accountID; }
  FCULONG GetID() const                   { return m_id; }
  string GetName() const                  { return m_name; }
  string GetEmail() const                 { return m_email; }
  FCULONG GetXP() const                   { return m_xp; }
  FCULONG GetLevel() const                { return m_level; }
  FCINT GetFameScale() const              { return m_fameScale; }
  FCULONG GetCityID() const               { return m_cityID; }
  FCULONG GetCountryID() const            { return m_countryID; }
  Computer& GetComputer()                 { return *m_computer; }
  InGameIPAddress& GetIP()                { return m_ip; }
  FCSOCKET GetClientSocket() const        { return m_clientSocket; }
	BaseSocket* GetRouterSocket()						{ return m_pRouterSocket; }

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
	void SetRouterSocket(BaseSocket* pSock)	{ m_pRouterSocket = pSock; }

private:

  /*
   *  Private Methods
   */
  void createComputer();

  /*
   *  Private Members
   */
  FCULONG m_accountID;
  FCULONG m_id;
  string m_name;
  string m_email;
  FCULONG m_xp;
  FCULONG m_level;
  FCINT m_fameScale;
  FCULONG m_cityID;
  FCULONG m_countryID;
  Computer* m_computer;
  InGameIPAddress m_ip;
  FCSOCKET m_clientSocket;
	BaseSocket* m_pRouterSocket;

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