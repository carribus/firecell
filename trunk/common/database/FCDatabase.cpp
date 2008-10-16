#include <algorithm>
#include "IDBInterface.h"
#include "DBIMySql.h"
#include ".\fcdatabase.h"

FCDatabase::FCDatabase(void)
: m_pDBI(NULL)
, m_uiNumThreads(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDatabase::~FCDatabase(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::Initialise(string strEngine, string server, string dbName, string user, string pass, FCUINT uiNumWorkerThreads)
{
  // instantiate the required db interface for the specfic database engine.
  if ( !(m_pDBI = CreateInterface(strEngine)) )
    return false;

  m_strServer = server;
  m_strDBName = dbName;
  m_strUser = user;
  m_strPass = pass;

  StartWorkerThreads(uiNumWorkerThreads);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

IDBInterface* FCDatabase::CreateInterface(string strEngine)
{
  IDBInterface* pi = NULL;

  // make the key lower case
  std::transform(strEngine.begin(), strEngine.end(), strEngine.begin(), (int(*)(int))tolower);
  if ( !strEngine.compare("mysql") )
  {
    pi = new DBIMySql;
  }
  else
  {
    /* no action to be taken when we don't have an engine */
  }

  return pi;
}

//////////////////////////////////////////////////////////////////////////////////////////

void FCDatabase::StartWorkerThreads(FCUINT uiNumThreads)
{
  WorkerThread* pThread = NULL;

  for ( FCUINT i = 0; i < uiNumThreads; i++ )
  {
    pThread = new WorkerThread;
    pThread->pThis = this;

    if ( pthread_create( &pThread->thread, NULL, thrdDBWorker, (void*)pThread ) != 0 )
    {
      // failed to create a worker thread :(
      delete pThread;
    }
    else
    {
      m_threads.push_back(pThread);
      m_uiNumThreads++;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void* FCDatabase::thrdDBWorker(void* pData)
{
  if ( !pData )
    return NULL;

  WorkerThread* pThreadObj = (WorkerThread*)pData;
  FCDatabase* pThis = pThreadObj->pThis;
  IDBInterface* pDB = pThis->m_pDBI;

  if ( !pDB )
    return NULL;

  // attempt to connect to the database
  if ( !pDB->Connect(pThis->m_strServer, 0, pThis->m_strDBName, pThis->m_strUser, pThis->m_strPass) )
  {
    // failed to connect...
    return NULL;
  }

  pThreadObj->bRunning = true;

  while ( pThreadObj->bRunning )
  {
    // check if there are any jobs for us to execute...
    // TODO: Complete the code :)

#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
  }

  return NULL;
}