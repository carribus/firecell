#ifndef _DBIRESULTS_H_
#define _DBIRESULTS_H_

#include <queue>
#include "DBIResultSet.h"

using namespace std;

class DBIResults
{
public:
  DBIResults(void);
  ~DBIResults(void);

  void AddResultSet(DBIResultSet* pResults);
  DBIResultSet* GetNextResultSet();
  size_t GetCount();
  void SetAffectedRows(unsigned long rows)                    { m_affectedRows = rows; }
  unsigned long GetAffectedRows()                             { return m_affectedRows; }

private:

  queue<DBIResultSet*>     m_resultSets;
  unsigned long m_affectedRows;
};

#endif//_DBIMYSQLRESULTS_H_