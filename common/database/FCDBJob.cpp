#include "FCDBJob.h"

FCDBJob::FCDBJob(const string& strQuery, void* pData)
: m_strQuery(strQuery)
, m_pData(pData)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDBJob::~FCDBJob(void)
{
}
