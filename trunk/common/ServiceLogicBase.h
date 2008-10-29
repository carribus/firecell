#ifndef _SERVICELOGICBASE_H_
#define _SERVICELOGICBASE_H_

#include <string>
#include "inifile.h"
#include "../common/database/FCDatabase.h"
#include "interfaces/IServiceLogic.h"

using namespace std;

class ServiceLogicBase : public IServiceLogic
{
public:
  ServiceLogicBase(const string& serviceName, bool bHasConsole);
  virtual ~ServiceLogicBase(void);

  /*
   *  IServiceLogic implementation
   */
  const char* GetName();
  void HasConsole(bool bHasConsole)               { m_bHasConsole = bHasConsole; }

protected:

  bool HasConsole()                               { return m_bHasConsole; }

  bool LoadConfig(FCCSTR strFilename);
  bool LoadDBSettingsFromConfig(string& strEngine, string& strServer, string& strDBName, string& strUser, string& strPass);

  bool ConfigureDatabase(string strEngine, string strServer, string strDBName, string strUser, string strPass);
  static void* thrdDBWorker(void* pData);
  void HandleCompletedDBJob(FCDBJob& job);

  /*
   *  Protected members
   */
  INIFile m_config;

private:

  string m_ServiceName;
  bool m_bHasConsole;

  // Database related members
  FCDatabase m_db;
  pthread_t m_thrdDBMon;
  bool m_bDBMonRunning;
};

#endif//_SERVICELOGICBASE_H_