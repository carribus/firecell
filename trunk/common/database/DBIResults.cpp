#include "DBIResults.h"

DBIResults::DBIResults(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIResults::~DBIResults(void)
{
  while ( m_resultSets.size() )
  {
    delete m_resultSets.front();
    m_resultSets.pop();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIResults::AddResultSet(DBIResultSet* pResults)
{
  if ( !pResults )
    return;

  m_resultSets.push(pResults);
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIResultSet* DBIResults::GetNextResultSet()
{
  if ( !m_resultSets.size() )
    return NULL;

  DBIResultSet* pResults = m_resultSets.front();
  m_resultSets.pop();

  return pResults;
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t DBIResults::GetCount()
{
  return m_resultSets.size();
}
