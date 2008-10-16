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
#ifndef _FCDATABASE_H_
#define _FCDATABASE_H_

#include <string>
#include <vector>
#include <queue>
#include "../fctypes.h"
#include "../fcqueries.h"
#include "../INIFile.h"
#include "../threading.h"
#include "FCDBJob.h"
#include "IDBInterface.h"

using namespace std;

class FCDatabase
{
public:
  FCDatabase(void);
  virtual ~FCDatabase(void);

  bool Initialise(string strEngine, string server, string dbName, string user, string pass, FCUINT uiNumWorkerThreads = 2);
  bool ExecuteJob(const string QueryName, void* pData, ...);

private:

  /*
   *  PRIVATE: Methods
   */
  IDBInterface* CreateInterface(string strEngine);
  FCUINT LoadQueries(string strEngine);
  void StartWorkerThreads(FCUINT uiNumThreads);
  void StopWorkerThreads();

  static void* thrdDBWorker(void* pData);

  /*
   *  PRIVATE: Attributes
   */
  IDBInterface* m_pDBI;
  string m_strServer,
         m_strDBName,
         m_strUser,
         m_strPass;

  // queries file for the selected engine
  INIFile m_queryFile;
  INIFile::CSection* m_pQueries;

  // number of worker threads active
  FCUINT m_uiNumThreads;

  // structure and vector for the worker threads
  struct WorkerThread
  {
    WorkerThread()
      : pThis(NULL)
      , bRunning(false)
    {
    }

    ~WorkerThread()
    {
    }

    pthread_t       thread;
    FCDatabase*     pThis;
    bool            bRunning;
  };
  vector<WorkerThread*> m_threads;

  // outstanding db jobs that have been queued
  pthread_mutex_t m_mutexJobs;
  queue<FCDBJob> m_jobs;
};

#endif//_FCDATABASE_H_