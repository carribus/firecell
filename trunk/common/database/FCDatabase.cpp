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
#include "../Logging/DynLog.h"
#include "IDBInterface.h"
#include "mysql/DBIMySql.h"
#include "FCDatabase.h"

FCDatabase::FCDatabase(void)
: m_pDBI(NULL)
, m_uiNumThreads(0)
, m_pQueries(NULL)
{
  pthread_mutex_init(&m_mutexJobs, NULL);
  pthread_mutex_init(&m_mutexCompletedJobs, NULL);
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
  pthread_mutex_destroy(&m_mutexCompletedJobs);
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

string FCDatabase::GetQueryByName(const string& QueryName)
{
  return m_pQueries->GetValue(QueryName);
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::ExecuteJob(const string QueryName, void* pData, ...)
{
  if ( !m_pQueries || QueryName.empty() )
    return false;

  if ( QueryName.empty() )
    DYNLOG_ADDLOG( DYNLOG_FORMAT("ExecuteJob: QueryName=%s", QueryName.c_str()) );

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

    QueueJob(sqlQuery, QueryName, pData);
    bResult = true;
  }

  return bResult;
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::ExecuteDirectQuery(const string query, const string ref, void* pData)
{
  if ( query.empty() )
    return false;

  QueueJob(query, ref, pData);

  return true;
}

//////////////////////////////////////////////////////////////////////////////////////////

size_t FCDatabase::GetCompletedJobCount()
{
  return m_completedJobs.size();
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::GetNextCompletedJob(FCDBJob& job)
{
  if ( m_completedJobs.size() == 0 )
    return false;

  pthread_mutex_lock(&m_mutexCompletedJobs);
  job = m_completedJobs.front();
  m_completedJobs.pop();
  pthread_mutex_unlock(&m_mutexCompletedJobs);

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
    DYNLOG_ADDLOG( "No database engine created. Failed to create interface." );
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

void FCDatabase::QueueJob(const string query, const string ref, void* pData)
{
  FCDBJob job(query, pData);

  job.SetReference(ref);

  pthread_mutex_lock(&m_mutexJobs);
  m_jobs.push(job);
  pthread_mutex_unlock(&m_mutexJobs);
}

//////////////////////////////////////////////////////////////////////////////////////////

void FCDatabase::JobComplete(FCDBJob& job)
{
  pthread_mutex_lock(&m_mutexCompletedJobs);

  m_completedJobs.push(job);

  pthread_mutex_unlock(&m_mutexCompletedJobs);
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::HandleError(unsigned int nError)
{
  bool bResult = false;

  switch ( nError )
  {
  case  2006:       // server has gone away
    {
      // try to reconnect to the server
      DYNLOG_ADDLOG("Attempting to reconnect to database...");
      if ( m_pDBI )
      {
        if ( m_pDBI->Connect(m_strServer, 0, m_strDBName, m_strUser, m_strPass) )
          DYNLOG_ADDLOG("Connect to database re-established");
      }
    }
    break;

  default:
    break;
  }

  return bResult;
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
  DBIResults* pResults;

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
      // synchronised job fetch
      pthread_mutex_lock(&pThis->m_mutexJobs);
      if ( pThis->m_jobs.size() == 0 )
      {
        pthread_mutex_unlock(&pThis->m_mutexJobs);
        continue;
      }

      FCDBJob job = pThis->m_jobs.front();
      pThis->m_jobs.pop();
      pthread_mutex_unlock(&pThis->m_mutexJobs);

      // execute the job
      if ( !job.GetQuery().empty() )
      {
        if ( (pResults = pConn->Execute(job)) )
        {
          job.SetResults(pResults);
          job.IsCompleted(true);
          // notify listener that a job has completed
          pThis->JobComplete(job);
        }
        else
        {
          // an error probably occurred here... lets check
          unsigned int nError = pConn->GetLastErrorNum();
          string strError = pConn->GetLastError();

          if ( strError.length() )
          {
            DYNLOG_ADDLOG( DYNLOG_FORMAT("FCDatabase Error [%ld]: %s", nError, strError.c_str()) );
          }

          if ( !pThis->HandleError(nError) )
            DYNLOG_ADDLOG("No specific error handling performed for this error");
        }
      }
      else
      {
        // empty job in queue
      }
    }


#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
  }

  // close the db connection
  pConn->Disconnect();

  return NULL;
}