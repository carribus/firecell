#include "MissionMgr.h"

MissionMgr::MissionMgr(void)
{
}

///////////////////////////////////////////////////////////////////////

MissionMgr::~MissionMgr(void)
{
}

///////////////////////////////////////////////////////////////////////

bool MissionMgr::addMission(FCULONG mission_id, bool bCompleted, FCULONG parent_mission_id)
{
  _Mission m;

  m.mission_id = mission_id;
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