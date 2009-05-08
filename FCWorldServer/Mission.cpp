#include "../common/Logging/DynLog.h"
#include "../common/database/FCDatabase.h"
#include "world_comms.h"
#include "EventSystem.h"
#include "Event.h"
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
, m_successReqCount(0)
, m_failureReqCount(0)
, m_successCount(0)
, m_failureCount(0)
, m_successXP(0)
, m_failureXP(0)
, m_bComplete(false)
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
	m_successReqCount = src.m_successReqCount;
	m_failureReqCount = src.m_failureReqCount;
	m_successCount = src.m_successCount;
	m_failureCount = src.m_failureCount;
  m_eventSuccess = src.m_eventSuccess;
  m_eventFailure = src.m_eventFailure;
  m_successXP = src.m_successXP;
  m_failureXP = src.m_failureXP;
	m_pEventSystem = src.m_pEventSystem;
	m_bComplete = false;
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

Mission* Mission::GetChildMissionByID(FCULONG mission_id)
{
	if ( !mission_id )
		return NULL;

	std::vector<Mission*>::iterator it = m_childMissions.begin();
	std::vector<Mission*>::iterator limit = m_childMissions.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetID() == mission_id )
			return (*it);
	}

	return NULL;
}

///////////////////////////////////////////////////////////////////////

bool Mission::IsMyChild(Mission* pMission)
{
	std::vector<Mission*>::iterator it = m_childMissions.begin();
	std::vector<Mission*>::iterator limit = m_childMissions.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it) == pMission )
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

void Mission::RegisterForEvents(IEventSystem* pEventSystem)
{
	if ( !pEventSystem )
		return;

  m_pEventSystem = pEventSystem;
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
		Mission* pMission = GetChildMissionByID( (FCULONG)pEvent->GetParam() );

		if ( !IsComplete() && pMission->IsComplete() )
		{
			// check if our success event is a collection of child mission completion events
			if ( m_eventSuccess.find("(child).") == 0 )
			{
				// ensure that we do not handle our own event
				// also check that the mission sending the completion event is one of our child mission objects
				if ( pSource != (IEventSource*)this && IsMyChild(pMission) )
				{
					// handle events
					if ( !eventCode.compare( Mission::EVT_Complete ) )
					{
						IncrementSuccessCount(pEvent->GetPlayer());
            // check if all child missions are complete
						if ( m_successCount == m_successReqCount || (m_successReqCount == 0 && m_successCount == m_childMissions.size()) )
						{
							SetComplete(true, pEvent->GetPlayer());
							// all our missions are complete.. therefore we are complete
							if ( pEvent->GetPlayer() )
							{
								// emit a mission completion event to the player object
								EventSystem::GetInstance()->Emit(this, (IEventTarget*)pEvent->GetPlayer(), new Event( Mission::EVT_Complete, (void*)m_id, pEvent->GetPlayer() ));
							}
						}
						else
						{
							pEvent->GetPlayer()->AddXP( pMission->GetSuccessXP() );
						}
					}
				}
			}
		}
  }
  else
  {
		bool bHandled = false;

		// check if this mission has already been completed
		// then check whether the mission was completed 
		if ( !IsComplete() && !m_eventSuccess.compare( eventCode ) )
		{
      IncrementSuccessCount(pEvent->GetPlayer());
			if ( m_successCount == m_successReqCount )
			{
				IEventTarget* pNextTarget = NULL;
				SetComplete(true, pEvent->GetPlayer());

				if ( m_pParentMission )
					pNextTarget = (IEventTarget*)m_pParentMission;
				else
					pNextTarget = (IEventTarget*)pEvent->GetPlayer();
				EventSystem::GetInstance()->Emit(this, pNextTarget, new Event( Mission::EVT_Complete, (void*)m_id, pEvent->GetPlayer() ));
			}
			bHandled = true;
		}
		else if ( !m_eventFailure.compare( eventCode) )
		{
			if ( m_pParentMission )
			{
        // TODO
			}
			else
			{
        // TODO
			}
			bHandled = true;
		}

		if ( !bHandled )
			DYNLOG_ADDLOG( DYNLOG_FORMAT("Mission::OnEvent(): Unknown event received [source:%s / event:%s]\n", sourceType.c_str(), pEvent->GetCode().c_str()) );
  }
}

///////////////////////////////////////////////////////////////////////

const std::string& Mission::GetType()
{
	return Mission::EVTSYS_ObjectType;
}

///////////////////////////////////////////////////////////////////////

void Mission::IncrementSuccessCount(Player* pPlayer)
{
  if ( !pPlayer )
    return;

  FCDatabase& db = FCDatabase::instance();

	m_successCount++;

  // update the mission's success count
  db.ExecuteJob(DBQ_SAVE_CHARACTER_MISSION_SUCCESS_COUNT, NULL, m_successCount, m_id, pPlayer->GetID());
}

///////////////////////////////////////////////////////////////////////

void Mission::SetComplete(bool bComplete, Player* pPlayer)
{
  if ( !pPlayer )
    return;

  FCDatabase& db = FCDatabase::instance();

  SetComplete(bComplete);

  // update the mission's success count
  db.ExecuteJob(DBQ_SAVE_CHARACTER_MISSION_COMPLETE_FLAG, NULL, bComplete ? "true" : "false", m_id, pPlayer->GetID());
}