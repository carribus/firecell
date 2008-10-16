#ifdef _WIN32
#include <windows.h>
#endif//_WIN32
#include <algorithm>
#include "IDBInterface.h"
#include "DBIMySql.h"
#include "fcdatabase.h"

FCDatabase::FCDatabase(void)
: m_pDBI(NULL)
, m_uiNumThreads(0)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDatabase::~FCDatabase(void)
{
  StopWorkerThreads();
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
    LoadQueries("mysql");
  }
  else
  {
    /* no action to be taken when we don't have an engine */
  }

  return pi;
}

//////////////////////////////////////////////////////////////////////////////////////////

FCUINT FCDatabase::LoadQueries(string strEngine)
{
  FCUINT uiCount = 0;
  string filename = "queries_" + strEngine + ".conf";
  INIFile ini;

  if ( ini.Load( filename.c_str() ) == 0 )
  {
    INIFile::CSection* pSection = ini.GetSection( strEngine );

    if ( pSection )
    {
      uiCount = (FCUINT) pSection->GetCount();
    }
  }

  return uiCount;
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

void FCDatabase::StopWorkerThreads()
{
  vector<WorkerThread*>::iterator it;

  for ( it = m_threads.begin(); it != m_threads.end(); it++ )
  {
    (*it)->bRunning = false;
    pthread_join( (*it)->thread, NULL );
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