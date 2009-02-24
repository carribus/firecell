#include "MissionManager.h"

MissionManager::MissionManager(IEventSystem* pEventSystem)
: m_pEventSystem(pEventSystem)
{
}

///////////////////////////////////////////////////////////////////////

MissionManager::~MissionManager(void)
{
	MissionMap::iterator it = m_mapMissions.begin();
	MissionMap::iterator limit = m_mapMissions.end();

	for ( ; it != limit; it++ )
	{
		delete it->second;
	}
}

///////////////////////////////////////////////////////////////////////

bool MissionManager::AddMission(FCULONG missionID, FCSHORT minLevel, FCSHORT maxLevel, FCSHORT difficulty, string success_event, string failure_event, FCSHORT xpSuccess, FCSHORT xpFailure, FCULONG parent_id, FCSHORT successCount, FCSHORT failureCount)
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
    pMission->SetSuccessXP(xpSuccess);
    pMission->SetFailureXP(xpFailure);

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

///////////////////////////////////////////////////////////////////////

FCULONG MissionManager::GetAvailableMissionsForPlayer(Player* pPlayer, vector<Mission*>& target)
{
	if ( !pPlayer )
		return 0;

	MissionMap::iterator it = m_mapMissions.begin();
	Mission* pMission = NULL;

	while ( it != m_mapMissions.end() )
	{
		pMission = it->second;
		// check if the player's level allows the taking of this mission
		if ( pPlayer->GetLevel() >= (FCULONG)pMission->GetMinLevel() &&
			   (pMission->GetMaxLevel() != 0 ? pPlayer->GetLevel() <= (FCULONG)pMission->GetMaxLevel() : true) )
		{
			// check if the player has completed the mission
			if ( !pPlayer->HasCompletedMission(pMission->GetID()) )
			{
				target.push_back(pMission);
			}
		}
	}

	return (FCULONG)target.size();
}

///////////////////////////////////////////////////////////////////////

bool MissionManager::AssignMissionToPlayer(Player* pPlayer, FCULONG mission_id)
{
	if ( !pPlayer || !mission_id )
		return false;

	MissionMap::iterator it = m_mapMissions.begin();
	MissionMap::iterator limit = m_mapMissions.end();

	for ( ; it != limit; it++ )
	{
		if ( it->second->GetID() == mission_id )
			pPlayer->AcceptMission(it->second);
		else if ( it->second->GetParentID() == mission_id )
			AssignMissionToPlayer(pPlayer, it->second->GetID());
	}

	return true;
}