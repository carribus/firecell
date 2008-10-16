#include <algorithm>
#include "IDBInterface.h"
#include "DBIMySql.h"
#include ".\fcdatabase.h"

FCDatabase::FCDatabase(void)
: m_pDBI(NULL)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

FCDatabase::~FCDatabase(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

bool FCDatabase::Initialise(string strEngine, string strDBName, string strUser, string strPass)
{
  m_strEngine = strEngine;
  m_strUser = strUser;
  m_strPass = strPass;

  // instantiate the required db interface for the specfic database engine.
  if ( !(m_pDBI = CreateInterface(strEngine)) )
    return false;

  

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
  }

  return pi;
}