#include "MissionManager.h"

MissionManager::MissionManager(void)
{
}

///////////////////////////////////////////////////////////////////////

MissionManager::~MissionManager(void)
{
}

///////////////////////////////////////////////////////////////////////

bool MissionManager::AddMission(FCULONG missionID, FCSHORT minLevel, FCSHORT maxLevel, FCSHORT difficulty, string success_event, string failure_event, FCULONG parent_id, FCSHORT successCount, FCSHORT failureCount)
{
  Mission* pMission = GetMission(missionID);

  if ( !pMission )
  {
    pMission = new Mission;
    pMission->SetID(missionID);
    pMission->SetParentID(parent_id);
    pMission->SetMinLevel(minLevel);
    pMission->SetMaxLevel(maxLevel);
    pMission->SetDifficulty(difficulty);
    pMission->SetEventSuccess(success_event);
    pMission->SetEventFailure(failure_event);
    pMission->SetSuccessCount(successCount);
    pMission->SetFailureCount(failureCount);

    m_mapMissions[missionID] = pMission;
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

Mission* MissionManager::GetMission(FCULONG missionID)
{
  MissionMap::iterator it = m_mapMissions.find(missionID);

  if ( it != m_mapMissions.end() )
    return it->second;

  return NULL;
}