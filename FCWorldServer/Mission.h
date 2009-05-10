#ifndef _MISSION_H_
#define _MISSION_H_

#include <vector>
#include "../common/fctypes.h"
#include "IEventSystem.h"
#include <string>

using namespace std;

class Player;

class Mission : public IEventSource
							, public IEventTarget
{
public:

  DECLARE_EVENT_SOURCE();
  DECLARE_EVENT(Complete);

  Mission(void);
	Mission(const Mission& src);
  ~Mission(void);

	bool CanPlayerAccept(Player* pPlayer);
	Mission* Clone();
  void AddChildMission(Mission* pMission);
  size_t GetChildCount()                                  { return m_childMissions.size(); }
  Mission* GetChildMission(size_t index);
	Mission* GetChildMissionByID(FCULONG mission_id);
	bool IsMyChild(Mission* pMission);

  FCULONG GetID()                                         { return m_id; }
  FCULONG GetParentID()                                   { return m_parentID; }
  FCSHORT GetMinLevel()                                   { return m_minLevel; }
  FCSHORT GetMaxLevel()                                   { return m_maxLevel; }
  FCSHORT GetDifficulty()                                 { return m_difficulty; }
  string GetEventSuccess()                                { return m_eventSuccess; }
  string GetEventFailure()                                { return m_eventFailure; }
  FCSHORT GetRequiredSuccessCount()                       { return m_successReqCount; }
  FCSHORT GetRequiredFailureCount()                       { return m_failureReqCount; }
  FCSHORT GetSuccessCount()                               { return m_successCount; }
  FCSHORT GetFailureCount()                               { return m_failureCount; }
  FCSHORT GetSuccessXP()                                  { return m_successXP; }
  FCSHORT GetFailureXP()                                  { return m_failureXP; }
	bool IsComplete()																				{ return m_bComplete; }
  Mission* GetParentMission()                             { return m_pParentMission; }

  void SetID(FCULONG id)                                  { m_id = id; }
  void SetParentID(FCULONG id)                            { m_parentID = id; }
  void SetMinLevel(FCSHORT lvl)                           { m_minLevel = lvl; }
  void SetMaxLevel(FCSHORT lvl)                           { m_maxLevel = lvl; }
  void SetDifficulty(FCSHORT diff)                        { m_difficulty = diff; }
  void SetEventSuccess(string event)                      { m_eventSuccess = event; }
  void SetEventFailure(string event)                      { m_eventFailure = event; }
  void SetRequiredSuccessCount(FCSHORT count)             { m_successReqCount = count; }
  void SetRequiredFailureCount(FCSHORT count)             { m_failureReqCount = count; }
  void SetSuccessCount(FCSHORT count)                     { m_successCount = count; }
  void SetFailureCount(FCSHORT count)                     { m_failureCount = count; }
  void SetSuccessXP(FCSHORT xp)                           { m_successXP = xp; }
  void SetFailureXP(FCSHORT xp)                           { m_failureXP = xp; }
	void SetComplete(bool bComplete = true)									{ m_bComplete = bComplete; }
  void SetParentMission(Mission* pParent)                 { m_pParentMission = pParent; }

  /*
   *	IEventSource/IEventTarget implementation
   */
  void RegisterForEvents(IEventSystem* pEventSystem);
  void OnEvent(IEventSource* pSource, IEvent* pEvent);
  const std::string& GetType();

private:

  void IncrementSuccessCount(Player* pPlayer);
  void SetComplete(bool bComplete, Player* pPlayer);

  FCULONG m_id;
  FCULONG m_parentID;
  FCSHORT m_minLevel;
  FCSHORT m_maxLevel;
  FCSHORT m_difficulty;
  string m_eventSuccess;
  string m_eventFailure;
  FCSHORT m_successReqCount;
  FCSHORT m_failureReqCount;
  FCSHORT m_successCount;
  FCSHORT m_failureCount;
  FCSHORT m_successXP;
  FCSHORT m_failureXP;
	bool m_bComplete;

	IEventSystem* m_pEventSystem;

	Mission* m_pParentMission;
  std::vector<Mission*> m_childMissions;
};

#endif//_MISSION_H_