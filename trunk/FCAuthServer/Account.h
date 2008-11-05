#ifndef _ACCOUNT_H_
#define _ACCOUNT_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class Account
{
public:
  Account(void);
  ~Account(void);

  FCUINT GetID()                             { return m_nID; }
  string GetName()                           { return m_name; }
  FCUINT GetType()                           { return m_nType; }
  FCUINT GetCurrentCharacterID()             { return m_currentCharacterID; }
  FCUINT GetClientSocket()                   { return m_clientSocket; }
  void SetID(FCUINT nID)                     { m_nID = nID; }
  void SetName(string name)                  { m_name = name; }
  void SetType(FCUINT nType)                 { m_nType = nType; }
  void SetClientSocket(FCUINT clientSocket)  { m_clientSocket = clientSocket; }
  void SetCurrentCharacterID(FCUINT id)      { m_currentCharacterID = id; }

private:

  FCUINT m_nID;
  string m_name;
  FCUINT m_nType;
  FCUINT m_currentCharacterID;
  FCUINT m_clientSocket;
};

#endif//_ACCOUNT_H_