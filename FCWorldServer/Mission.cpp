#include "Mission.h"

Mission::Mission(void)
: m_id(0)
, m_parentID(0)
, m_minLevel(1)
, m_maxLevel(100)
, m_difficulty(1)
, m_successCount(0)
, m_failureCount(0)
{
}

Mission::~Mission(void)
{
}
