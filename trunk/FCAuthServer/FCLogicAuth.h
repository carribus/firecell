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

  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  /*
   *  DB Job Handlers
   */
  static void OnDBJob_LoadAccount(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadCharacterIDs(DBIResultSet& resultSet, void*& pContext);
};

#endif//_FCLOGICAUTH_H_