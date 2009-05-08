#include "MissionManager.h"

MissionManager::MissionManager(IEventSystem* pEventSystem)
: m_pEventSystem(pEventSystem)
{
}

///////////////////////////////////////////////////////////////////////

MissionManager::~MissionManager(void)
{
  m_missionLock.LockForWrite();
	MissionMap::iterator it = m_mapMissions.begin();
	MissionMap::iterator limit = m_mapMissions.end();

	for ( ; it != limit; it++ )
	{
		delete it->second;
	}
  m_missionLock.Unlock();
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

    m_missionLock.LockForWrite();
    m_mapMissions[missionID] = pMission;
    m_missionLock.Unlock();
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

Mission* MissionManager::GetMission(FCULONG missionID)
{
  m_missionLock.LockForRead();
  MissionMap::iterator it = m_mapMissions.find(missionID);

  if ( it != m_mapMissions.end() )
  {
    Mission* pMission = it->second;
    m_missionLock.Unlock();
    return pMission;
  }

  m_missionLock.Unlock();

  return NULL;
}

///////////////////////////////////////////////////////////////////////

FCULONG MissionManager::GetAvailableMissionsForPlayer(Player* pPlayer, vector<Mission*>& target)
{
	if ( !pPlayer )
		return 0;

  m_missionLock.LockForRead();
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

  m_missionLock.Unlock();

	return (FCULONG)target.size();
}

///////////////////////////////////////////////////////////////////////

bool MissionManager::AssignMissionToPlayer(Player* pPlayer, FCULONG mission_id)
{
	if ( !pPlayer || !mission_id )
		return false;

  m_missionLock.LockForRead();

	MissionMap::iterator it = m_mapMissions.begin();
	MissionMap::iterator limit = m_mapMissions.end();

	for ( ; it != limit; it++ )
	{
		if ( it->second->GetID() == mission_id )
			pPlayer->AcceptMission(it->second);
		else if ( it->second->GetParentID() == mission_id )
			AssignMissionToPlayer(pPlayer, it->second->GetID());
	}

  m_missionLock.Unlock();

	return true;
}