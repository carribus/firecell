#ifndef _FCDATABASE_H_
#define _FCDATABASE_H_

#include <string>
#include <vector>
#include "../fctypes.h"
#include "../threading.h"
#include "IDBInterface.h"

using namespace std;

class FCDatabase
{
public:
  FCDatabase(void);
  virtual ~FCDatabase(void);

  bool Initialise(string strEngine, string server, string dbName, string user, string pass, FCUINT uiNumWorkerThreads = 2);

private:

  /*
   *  PRIVATE: Methods
   */
  IDBInterface* CreateInterface(string strEngine);
  void StartWorkerThreads(FCUINT uiNumThreads);

  static void* thrdDBWorker(void* pData);

  /*
   *  PRIVATE: Attributes
   */
  IDBInterface* m_pDBI;
  string m_strServer,
         m_strDBName,
         m_strUser,
         m_strPass;

  FCUINT m_uiNumThreads;

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

};

#endif//_FCDATABASE_H_