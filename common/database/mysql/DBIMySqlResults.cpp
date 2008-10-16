#include "DBIMySqlResults.h"

DBIMySqlResults::DBIMySqlResults(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

DBIMySqlResults::~DBIMySqlResults(void)
{
  while ( m_resultSets.size() )
  {
    delete m_resultSets.front();
    m_resultSets.pop();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void DBIMySqlResults::AddResultSet(DBIMySqlResultSet* pResults)
{
  if ( !pResults )
    return;

  m_resultSets.push(pResults);
}