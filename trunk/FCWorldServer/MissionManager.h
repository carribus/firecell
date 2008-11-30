#ifndef _MISSIONMANAGER_H_
#define _MISSIONMANAGER_H_

#include <map>
#include "Mission.h"

class MissionManager
{
public:
  MissionManager(void);
  ~MissionManager(void);

  bool AddMission(FCULONG missionID, FCSHORT minLevel, FCSHORT maxLevel, FCSHORT difficulty, string success_event, string failure_event, FCULONG parent_id = 0, FCSHORT successCount = 1, FCSHORT failureCount = 1);
  Mission* GetMission(FCULONG missionID);

private:

  typedef map<FCULONG, Mission*> MissionMap;
  MissionMap m_mapMissions;
};

#endif//_MISSIONMANAGER_H_