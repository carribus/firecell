#include "FCDBJob.h"

FCDBJob::FCDBJob(const string& strQuery, void* pData)
: m_strQuery(strQuery)
, m_pData(pData)
, m_bCompleted(false)
, m_pResults(NULL)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDBJob::~FCDBJob(void)
{
}
