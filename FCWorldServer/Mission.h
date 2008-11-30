#ifndef _MISSION_H_
#define _MISSION_H_

#include "../common/fctypes.h"
#include <string>

using namespace std;

class Mission
{
public:
  Mission(void);
  ~Mission(void);

  FCULONG GetID()                                         { return m_id; }
  FCULONG GetParentID()                                   { return m_parentID; }
  FCSHORT GetMinLevel()                                   { return m_minLevel; }
  FCSHORT GetMaxLevel()                                   { return m_maxLevel; }
  FCSHORT GetDifficulty()                                 { return m_difficulty; }
  string GetEventSuccess()                                { return m_eventSuccess; }
  string GetEventFailure()                                { return m_eventFailure; }
  FCSHORT GetSuccessCount()                               { return m_successCount; }
  FCSHORT GetFailureCount()                               { return m_failureCount; }

  void SetID(FCULONG id)                                  { m_id = id; }
  void SetParentID(FCULONG id)                            { m_parentID = id; }
  void SetMinLevel(FCSHORT lvl)                           { m_minLevel = lvl; }
  void SetMaxLevel(FCSHORT lvl)                           { m_maxLevel = lvl; }
  void SetDifficulty(FCSHORT diff)                        { m_difficulty = diff; }
  void SetEventSuccess(string event)                      { m_eventSuccess = event; }
  void SetEventFailure(string event)                      { m_eventFailure = event; }
  void SetSuccessCount(FCSHORT count)                     { m_successCount = count; }
  void SetFailureCount(FCSHORT count)                     { m_failureCount = count; }

private:

  FCULONG m_id;
  FCULONG m_parentID;
  FCSHORT m_minLevel;
  FCSHORT m_maxLevel;
  FCSHORT m_difficulty;
  string m_eventSuccess;
  string m_eventFailure;
  FCSHORT m_successCount;
  FCSHORT m_failureCount;
};

#endif//_MISSION_H_