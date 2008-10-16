#ifndef _FCDBJOB_H_
#define _FCDBJOB_H_

#include <string>

using namespace std;

class FCDBJob
{
public:
  FCDBJob(const string& strQuery, void* pData = NULL);
  virtual ~FCDBJob(void);

  string GetQuery()                       { return m_strQuery; }
  void* GetData()                         { return m_pData; }

private:

  string m_strQuery;
  void* m_pData;
};

#endif//_FCDBJOB_H_