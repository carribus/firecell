#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include <map>
#include "../common/PThreadRWLock.h"
#include "IEventSystem.h"
#include "Mission.h"
#include "Player.h"

class MissionManager
{
public:
  MissionManager(IEventSystem* pEventSystem = NULL);
  ~MissionManager(void);

  bool AddMission(FCULONG missionID, FCSHORT minLevel, FCSHORT maxLevel, FCSHORT difficulty, string success_event, string failure_event, FCSHORT xpSuccess, FCSHORT xpFailure, FCULONG parent_id = 0, FCSHORT successCount = 1, FCSHORT failureCount = 1);
  Mission* GetMission(FCULONG missionID);
	FCULONG GetAvailableMissionsForPlayer(Player* pPlayer, vector<Mission*>& target);
	bool AssignMissionToPlayer(Player* pPlayer, FCULONG mission_id);

  void SetEventSystem(IEventSystem* pES)                { m_pEventSystem = pES; }

private:

	IEventSystem* m_pEventSystem;
  typedef map<FCULONG, Mission*> MissionMap;
  MissionMap m_mapMissions;
  PThreadRWLock m_missionLock;
};

#endif//_MISSIONMANAGER_H_