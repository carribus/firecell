#ifndef _DBIMYSQLRESULTS_H_
#define _DBIMYSQLRESULTS_H_

#include <queue>
#include "DBIMySqlResultSet.h"

using namespace std;

class DBIMySqlResults
{
public:
  DBIMySqlResults(void);
  ~DBIMySqlResults(void);

  void AddResultSet(DBIMySqlResultSet* pResults);

private:

  queue<DBIMySqlResultSet*>     m_resultSets;
};

#endif//_DBIMYSQLRESULTS_H_