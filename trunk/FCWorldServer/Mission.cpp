#include "../common/Logging/DynLog.h"
#include "Player.h"
#include "Mission.h"

DEFINE_EVENT_SOURCE(Mission);
DEFINE_EVENT(Mission, Complete);

Mission::Mission(void)
: m_id(0)
, m_parentID(0)
, m_minLevel(1)
, m_maxLevel(100)
, m_difficulty(1)
, m_successCount(0)
, m_failureCount(0)
, m_pEventSystem(NULL)
, m_pParentMission(NULL)
{
}

///////////////////////////////////////////////////////////////////////

Mission::Mission(const Mission& src)
{
	m_id = src.m_id;
	m_parentID = src.m_parentID;
	m_minLevel = src.m_minLevel;
	m_maxLevel = src.m_maxLevel;
	m_difficulty = src.m_difficulty;
	m_successCount = src.m_successCount;
	m_failureCount = src.m_failureCount;
  m_eventSuccess = src.m_eventSuccess;
  m_eventFailure = src.m_eventFailure;
	m_pEventSystem = src.m_pEventSystem;
  m_pParentMission = NULL;
}

///////////////////////////////////////////////////////////////////////

Mission::~Mission(void)
{
  if ( m_pEventSystem )
  {
    m_pEventSystem->UnregisterEventTarget(this, Mission::EVT_Complete);
  }
}

///////////////////////////////////////////////////////////////////////

bool Mission::CanPlayerAccept(Player* pPlayer)
{
	// check level requirements
  if ( (FCULONG)m_minLevel <= pPlayer->GetLevel() && ( m_maxLevel > 0 ? (FCULONG)m_maxLevel >= pPlayer->GetLevel() : 1 ) )
	{
		if ( !pPlayer->HasMission(m_id) && !pPlayer->HasCompletedMission(m_id) )
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

Mission* Mission::Clone()
{
	Mission* pMission = new Mission(*this);

  return pMission;
}

///////////////////////////////////////////////////////////////////////

void Mission::AddChildMission(Mission* pMission)
{
  if ( pMission )
  {
    m_childMissions.push_back(pMission);
    pMission->SetParentMission(this);
  }
}

///////////////////////////////////////////////////////////////////////

Mission* Mission::GetChildMission(size_t index)
{
  if ( index >= m_childMissions.size() )
    return NULL;

  return m_childMissions[index];
}

///////////////////////////////////////////////////////////////////////

void Mission::RegisterForEvents(IEventSystem* pEventSystem)
{
	if ( !pEventSystem )
		return;

  m_pEventSystem = pEventSystem;

  // test listener
//	pEventSystem->RegisterEventTarget(this, Mission::EVT_Complete);
}

///////////////////////////////////////////////////////////////////////

void Mission::OnEvent(IEventSource* pSource, IEvent* pEvent)
{
  string sourceType = (pSource ? pSource->GetType() : "World");
  string eventCode = pEvent->GetCode();
  string eventName = eventCode.substr(eventCode.find('.')+1, eventCode.length());

  DYNLOG_ADDLOG( DYNLOG_FORMAT("Mission::OnEvent(): Event [%s] received from source [%s]\n", eventCode.c_str(), sourceType.c_str()) );

  if ( !sourceType.compare( Mission::EVTSYS_ObjectType ) )      // Mission events
  {
		Mission* pMission = (Mission*)pEvent->GetParam();

    // ensure that we do not handle our own event
    if ( pSource != (IEventSource*)this )
    {
      // handle events
      if ( !eventCode.compare( Mission::EVT_Complete ) )
      {
      }
    }
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Mission::OnEvent(): Unknown event received [source:%s / event:%s]\n", sourceType.c_str(), pEvent->GetCode().c_str()) );
  }
}

///////////////////////////////////////////////////////////////////////

const std::string& Mission::GetType()
{
	return Mission::EVTSYS_ObjectType;
}
