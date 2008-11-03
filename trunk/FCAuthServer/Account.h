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

  int GetID()                             { return m_nID; }
  string GetName()                        { return m_name; }
  int GetType()                           { return m_nType; }
  FCUINT GetClientSocket()                { return m_clientSocket; }
  void SetID(int nID)                     { m_nID = nID; }
  void SetName(string name)               { m_name = name; }
  void SetType(int nType)                 { m_nType = nType; }
  void SetClientSocket(FCUINT clientSocket) { m_clientSocket = clientSocket; }

private:

  int m_nID;
  string m_name;
  int m_nType;
  FCUINT m_clientSocket;
};

#endif//_ACCOUNT_H_