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

private:

  queue<DBIResultSet*>     m_resultSets;
};

#endif//_DBIMYSQLRESULTS_H_