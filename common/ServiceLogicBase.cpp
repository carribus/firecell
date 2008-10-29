
#ifdef _WIN32
#include <windows.h>
#endif//_WIN32
#include "ServiceLogicBase.h"

ServiceLogicBase::ServiceLogicBase(const string& serviceName, bool bHasConsole)
: m_ServiceName(serviceName)
, m_bHasConsole(bHasConsole)
{
}

///////////////////////////////////////////////////////////////////////

ServiceLogicBase::~ServiceLogicBase(void)
{
}

///////////////////////////////////////////////////////////////////////

const char* ServiceLogicBase::GetName()
{
  return m_ServiceName.c_str();
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::LoadDBSettingsFromConfig(string& strEngine, string& strServer, string& strDBName, string& strUser, string& strPass)
{
  bool bResult = false;
  INIFile::CSection* pSection = m_config.GetSection("Database");

  if ( pSection )
  {
    strEngine = pSection->GetValue("engine");
    strServer = pSection->GetValue("server");
    strDBName = pSection->GetValue("dbname");
    strUser = pSection->GetValue("user");
    strPass = pSection->GetValue("pass");
    bResult = true;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ServiceLogicBase::ConfigureDatabase(string strEngine, string strServer, string strDBName, string strUser, string strPass)
{
  // start the database monitoring thread to monitor for any results that may become available
  if ( pthread_create( &m_thrdDBMon, NULL, thrdDBWorker, (void*)this ) != 0 )
  {
    return false;
  }

  return m_db.Initialise(strEngine, strServer, strDBName, strUser, strPass);
}

///////////////////////////////////////////////////////////////////////

void* ServiceLogicBase::thrdDBWorker(void* pData)
{
  ServiceLogicBase* pThis = (ServiceLogicBase*)pData;
  FCDBJob job;

  if ( !pThis )
    return NULL;

  pThis->m_bDBMonRunning = true;

  while ( pThis->m_bDBMonRunning )
  {
    while ( pThis->m_db.GetCompletedJobCount() )
    {
      pThis->m_db.GetNextCompletedJob(job);
      pThis->HandleCompletedDBJob(job);
    }
#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////

void ServiceLogicBase::HandleCompletedDBJob(FCDBJob& job)
{
  string jobRef = job.GetReference();
  DBIResults* pResults = job.GetResults();
  DBIResultSet* pResultSet = NULL;
  DBJobContext* pCtx = (DBJobContext*)job.GetData();

  while ( pResults && pResults->GetCount() )
  {
    if ( (pResultSet = pResults->GetNextResultSet()) )
    {
      // TODO: Try and find a handler for this db context and forward it on....

      // clear the result set
      delete pResultSet;
    }
  }

  delete pCtx;
  delete pResults;
}
