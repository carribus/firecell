#ifndef _FCDATABASE_H_
#define _FCDATABASE_H_

#include <string>
#include "IDBInterface.h"

using namespace std;

class FCDatabase
{
public:
  FCDatabase(void);
  virtual ~FCDatabase(void);

  bool Initialise(string strEngine, string strDBName, string strUser, string strPass);

private:

  IDBInterface* CreateInterface(string strEngine);

  string m_strEngine;
  string m_strUser;
  string m_strPass;
  IDBInterface* m_pDBI;
};

#endif//_FCDATABASE_H_