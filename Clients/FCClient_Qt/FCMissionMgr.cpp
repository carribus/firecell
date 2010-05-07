#include "MissionMgr.h"

MissionMgr::MissionMgr(void)
{
}

///////////////////////////////////////////////////////////////////////

MissionMgr::~MissionMgr(void)
{
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::addMission(FCULONG mission_id, FCSHORT successCount, FCSHORT failureCount, bool bCompleted, FCULONG parent_mission_id)
{
  _Mission m;

  m.mission_id = mission_id;
  m.successCount = successCount;
  m.failureCount = failureCount;
  m.bCompleted = bCompleted;
  if ( !parent_mission_id )
  {
    // if this is mission that has no parent, then we add it to the root missions vector
    m_missions.push_back(m);
  }
  else
  {
    // if this mission is childed, lets find the parent
    _Mission* pParent = getMission(parent_mission_id);

    if ( pParent )
      pParent->children.push_back(m);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::completeMission(FCULONG mission_id)
{
	_Mission* pMission = getMission(mission_id);

	if ( !pMission )
		return false;

	pMission->bCompleted = true;

	return true;
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::removeMission(FCULONG mission_id)
{
	std::list<_Mission>::iterator it = m_missions.begin();
  std::list<_Mission>::iterator limit = m_missions.end();

  for ( ; it != limit; it++ )
  {
    if ( it->mission_id == mission_id )
		{
			m_missions.erase(it);
      return true;
		}
  }

	return false;
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::isMissionAccepted(FCULONG mission_id)
{
	std::list<_Mission>::iterator it = m_missions.begin();
  std::list<_Mission>::iterator limit = m_missions.end();

  for ( ; it != limit; it++ )
  {
    if ( it->mission_id == mission_id )
		{
      return true;
		}
  }

	return false;
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::isMissionComplete(FCULONG mission_id)
{
	std::list<_Mission>::iterator it = m_missions.begin();
  std::list<_Mission>::iterator limit = m_missions.end();

  for ( ; it != limit; it++ )
  {
    if ( it->mission_id == mission_id )
		{
      return it->bCompleted;
		}
  }

	return false;
}

///////////////////////////////////////////////////////////////////////

FCSHORT MissionMgr::getMissionSuccessCount(FCULONG mission_id)
{
  FCSHORT ret = 0;
  _Mission* pMission = getMission(mission_id);

  if ( pMission )
  {
    ret = pMission->successCount;
  }

  return ret;
}

///////////////////////////////////////////////////////////////////////

FCSHORT MissionMgr::getMissionFailureCount(FCULONG mission_id)
{
  FCSHORT ret = 0;
  _Mission* pMission = getMission(mission_id);

  if ( pMission )
  {
    ret = pMission->failureCount;
  }

  return ret;
}

///////////////////////////////////////////////////////////////////////

MissionMgr::_Mission* MissionMgr::getMission(FCULONG mission_id)
{
  std::list<_Mission>::iterator it = m_missions.begin();
  std::list<_Mission>::iterator limit = m_missions.end();

  for ( ; it != limit; it++ )
  {
    if ( it->mission_id == mission_id )
      return &(*it);
  }

  return NULL;
}