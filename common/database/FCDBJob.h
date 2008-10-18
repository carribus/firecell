#ifndef _FCDBJOB_H_
#define _FCDBJOB_H_

#include <string>
#include "DBIResults.h"

using namespace std;

class FCDBJob
{
public:
  FCDBJob(const string& strQuery = "", void* pData = NULL);
  virtual ~FCDBJob(void);

  void SetReference(string ref)           { m_strReference = ref; }
  void IsCompleted(bool bDone)            { m_bCompleted = bDone; }
  void SetResults(DBIResults* pRes)       { m_pResults = pRes; }

  string GetQuery()                       { return m_strQuery; }
  void* GetData()                         { return m_pData; }
  string GetReference()                   { return m_strReference; }
  bool IsCompleted()                      { return m_bCompleted; }
  DBIResults* GetResults()                { return m_pResults; }

private:

  string m_strReference;
  string m_strQuery;
  void* m_pData;
  bool m_bCompleted;
  DBIResults* m_pResults;
};

#endif//_FCDBJOB_H_