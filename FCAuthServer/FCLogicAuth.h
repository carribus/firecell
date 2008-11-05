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
#ifndef _FCLOGICAUTH_H_
#define _FCLOGICAUTH_H_

#include <string>
#include <map>
#include <queue>
#include "Account.h"
#include "../common/ServiceLogicBase.h"

using namespace std;

class FCLogicAuth : public ServiceLogicBase
{
  struct DBJobContext
  {
    FCLogicAuth* pThis;
    RouterSocket* pRouter;
    FCSOCKET clientSocket;
  };

public:
  FCLogicAuth(void);
  ~FCLogicAuth(void);

  ServiceType GetServiceType()                    { return ST_Auth; }

  //
  // IServiceLogic implementation
  void Free();
  int Start();
  int Stop();

private:

  bool IsAccountLoggedIn(int nAccountID);
  void AccountLogin(int nAccountID, string strAccountName, int nAccountType, FCUINT clientSocket);
  void AccountLogout(Account* pAccount);
  Account* GetAccountByClientSocket(FCUINT clientSocket);

  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnCommandLogin(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    bool OnCommandGetCharacters(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    bool OnCommandSelectCharacter(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    bool OnCommandClientDisconnect(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);

  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseRegisterService(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);

  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnErrorSelectCharacter(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);

  /*
   *  DB Job Handlers
   */
  static void OnDBJob_LoadAccount(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadCharacterInfo(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoginCharacter(DBIResultSet& resultSet, void*& pContext);

  /*
   *  Private Members
   */

  //
  // Map of accounts that are logged in
  typedef map<int, Account*> AccountMap;
  AccountMap m_mapLoggedInAccounts;
  pthread_mutex_t m_mutexLoggedInAccounts;
  //
  // Map of accounts to client sockets on the router
  typedef map<FCUINT, Account*> ClientSocketAccountMap;
  ClientSocketAccountMap m_mapSocketToAccount;
  pthread_mutex_t m_mutexSocketToAccount;
};

#endif//_FCLOGICAUTH_H_