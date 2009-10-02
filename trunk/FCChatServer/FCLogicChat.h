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
#ifndef _FCLOGICCHAT_H_
#define _FCLOGICCHAT_H_

#include <string>
#include <map>
#include <queue>
#include "../common/ServiceLogicBase.h"
#include "ChatRoomManager.h"

class FCLogicChat : public ServiceLogicBase
{
  struct DBJobContext
  {
    FCLogicChat* pThis;
    RouterSocket* pRouter;
    FCSOCKET clientSocket;
  };

public:
  FCLogicChat(void);
  ~FCLogicChat(void);

  ServiceType GetServiceType()                    { return ST_Chat; }

  //
  // IServiceLogic implementation
  void Free();
  int Start();
  int Stop();

private:

  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnCommandCharacterLoggedIn(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);
    bool OnCommandChatConnect(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);
    bool OnCommandChatListRooms(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);

  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);

  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  e_ChatConnectStatus CanPlayerConnect(Player* pPlayer);

  /*
   *  DB Job Handlers
   */
  static void OnDBJob_LoadChatRooms(DBIResultSet& resultSet, void*& pContext);

  /*
   *  Private members
   */
  PThreadCond           m_condSync;

  ChatRoomManager       m_chatRoomMgr;
};

#endif//_FCLOGICCHAT_H_