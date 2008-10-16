/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef _WIN32
#include <windows.h>
#endif//_WIN32
#include <algorithm>
#include "IDBInterface.h"
#include "mysql\DBIMySql.h"
#include "FCDatabase.h"

FCDatabase::FCDatabase(void)
: m_pDBI(NULL)
, m_uiNumThreads(0)
, m_pQueries(NULL)
{
  pthread_mutex_init(&m_mutexJobs, NULL);
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDatabase::~FCDatabase(void)
{
  if ( m_pDBI )
  {
    StopWorkerThreads();
    m_pDBI->Release();
  }

  pthread_mutex_destroy(&m_mutexJobs);
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

bool FCDatabase::ExecuteJob(const string QueryName, void* pData, ...)
{
  if ( !m_pQueries || QueryName.empty() )
    return false;

  bool bResult = false;
  string sqlQuery = m_pQueries->GetValue(QueryName);

  if ( !sqlQuery.empty() )
  {
    char buffer[32000];
    size_t queryLen = sqlQuery.length();
    // check if any variable args were passed in
    va_list marker = NULL;

    va_start( marker, pData );
    vsprintf(buffer, sqlQuery.c_str(), marker);
    va_end(marker);

    sqlQuery = buffer;
    FCDBJob job(sqlQuery, pData);

    pthread_mutex_lock(&m_mutexJobs);
    m_jobs.push(job);
    pthread_mutex_unlock(&m_mutexJobs);
    bResult = true;
  }

  return bResult;
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

  if ( m_queryFile.Load( filename.c_str() ) == 0 )
  {
    m_pQueries = m_queryFile.GetSection( strEngine );

    if ( m_pQueries )
    {
      uiCount = (FCUINT) m_pQueries->GetCount();
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
  IDBConnection* pConn = NULL;

  if ( !pDB )
    return NULL;

  // attempt to connect to the database
  if ( !(pConn = pDB->Connect(pThis->m_strServer, 0, pThis->m_strDBName, pThis->m_strUser, pThis->m_strPass)) )
  {
    // failed to connect...
    return NULL;
  }

  pThreadObj->bRunning = true;

  while ( pThreadObj->bRunning )
  {
    // check if there are any jobs for us to execute...
    while ( pThis->m_jobs.size() )
    {
      pthread_mutex_lock(&pThis->m_mutexJobs);

      FCDBJob job = pThis->m_jobs.front();
      pThis->m_jobs.pop();

      pthread_mutex_unlock(&pThis->m_mutexJobs);

      pConn->Execute(job.GetQuery());
    }


#ifdef _WIN32
    Sleep(1000);
#else
    usleep(250000);
#endif
  }

  // close the db connection
  pConn->Disconnect();

  return NULL;
}