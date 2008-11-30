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
#ifndef _FCLOGICWORLD_H_
#define _FCLOGICWORLD_H_

#include <string>
#include <map>
#include <queue>
#include "Player.h"
#include "PlayerManager.h"
#include "ItemManager.h"
#include "WorldManager.h"
#include "MissionManager.h"
#include "../common/ServiceLogicBase.h"
#include "../common/threading.h"

class FCLogicWorld : public ServiceLogicBase
{
  struct DBJobContext
  {
    DBJobContext() : pThis(NULL), pRouter(NULL), clientSocket(0), pData(NULL) {}
    FCLogicWorld* pThis;
    RouterSocket* pRouter;
    FCSOCKET clientSocket;
    void* pData;
  };

public:
  FCLogicWorld(void);
  ~FCLogicWorld(void);

  ServiceType GetServiceType()                    { return ST_World; }

  //
  // IServiceLogic implementation
  void Free();
  int Start();
  int Stop();

private:

  /*
   *  Private methods
   */
  void LoadWorldData();
  void ConfigureEventSystem();

  //
  // Packet sending functions
  void SendCharacterLoginStatus(FCULONG accountID, FCULONG characterID, e_SelectCharacterStatus status, RouterSocket* pRouter, FCSOCKET clientSocket);
  void SendCharacterAssetResponse(Player* pPlayer, RouterSocket* pRouter, FCSOCKET clientSocket);
  void SendCharacterDesktopOptions(Player* pPlayer, RouterSocket* pRouter, FCSOCKET clientSocket);
  void SendConsoleFileSystemInfo(FileSystem& fs, RouterSocket* pRouter, FCSOCKET clientSocket);
  void SendConsoleFileList(string currentDir, vector<FileSystem::File> files, RouterSocket* pRouter, FCSOCKET clientSocket);
  void SendConsoleCommandResult(Player* pPlayer, string result, RouterSocket* pRouter, FCSOCKET clientSocket);

  //
  // Packet handling functions
  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnCommandCharacterLoggedIn(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);
    bool OnCommandCharacterAssetRequest(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);
    bool OnCommandGetDesktopOptions(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket);
    // console commands
    bool OnCommandConsoleGetFSInfo(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    bool OnCommandConsoleGetFileList(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    bool OnCommandConsoleCommand(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);
    // forum commands
    bool OnCommandForumGetThreads(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);

    bool OnCommandClientDisconnect(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket);

  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  /*
   *  DB Job Handlers
   */
  static void OnDBJob_LoadItemTypes(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadItemDefs(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadObjectData(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadCharacterComputer(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadWorldGeography(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadCompanies(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadCompanyComputers(DBIResultSet& resultSet, void*& pContext);
  static void OnDBJob_LoadMissions(DBIResultSet& resultSet, void*& pContext);

  /*
   *  Helper functions
   */
  void UpdateComputerFromResultSet(Computer& comp, DBIResultSet& resultSet, size_t row = 0);

  /*
   *  Private members
   */
  string                m_pathFileSystems;

  PlayerManager         m_playerMgr;
  ItemManager           m_itemMgr;
  WorldManager          m_worldMgr;
  MissionManager        m_missionMgr;

  pthread_cond_t        m_condSync;
  pthread_mutex_t       m_mutexSync;
};

#endif//_FCLOGICWORLD_H_