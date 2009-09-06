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
#ifndef _FCCONTROLLER_H_
#define _FCCONTROLLER_H_

#include <string>
#include <queue>
#include <map>
#include "../../common/fctypes.h"
#include "../../common/PacketExtractor.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"

#include "IGameModule.h"
#include "ModConsole.h"
#include "ModForum.h"
#include "ModBank.h"
#include "ModChat.h"

using namespace std;

class FCController : public IBaseSocketSink
                   , public IGameModuleSink
{
  struct DataQueueItem
  {
    BaseSocket* pSocket;
    PEPacket* pPkt;
  };
  typedef queue<DataQueueItem> DataQueue;

  enum GameState
  {
    Loading = 0,
    Startup,
    LoginRequired,
    LoggingIn,
    Idle
  };

public:
  FCController(void);
  virtual ~FCController(void);

  bool Initialise(const string& username, const string& password);
  FCINT Run();

protected:

  bool ConnectToServer(string server, short port);
  void Disconnect();

  void QueueForAction();
  bool IsValidOption(int nOption);

  //
  // Data processing
  bool GetNextQueueItem(DataQueue& queue, DataQueueItem& dest);
  void ProcessIncomingData();
  void ProcessOutgoingData();

  //
  // IBaseSocketSink implementation
  void OnAccepted(BaseSocket* pSocket, int nErrorCode) {}
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  bool OnResponseServiceInfo(PEPacket* pPkt, BaseSocket* pSocket);  
  bool OnResponseLogin(PEPacket* pPkt, BaseSocket* pSocket);  
  bool OnResponseGetCharacters(PEPacket* pPkt, BaseSocket* pSocket);  
  bool OnResponseSelectCharacter(PEPacket* pPkt, BaseSocket* pSocket);  
  bool OnResponseCharacterItemsRequest(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponseCharacterAssetRequest(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponseGetDesktopOptions(PEPacket* pPkt, BaseSocket* pSocket);

  //
  // IGameModuleSink implementation
  void CloseModule(IGameModule* pModule);

private:

  string                  m_username,
                          m_password;

  GameState               m_gameState;
  FCUINT                  m_CurrentCharacterID;
  FCServerObj             m_server;
  BaseSocket              m_sock;
  bool                    m_bConnected;
  HANDLE                  m_hSockEvent;

  struct DesktopOption
  {
    FCULONG optionID;
    FCUINT type;
    char name[32];
  };
  map<FCUINT, DesktopOption>   m_desktopOptions;

  HANDLE                  m_hGameEvent;

  PacketExtractor         m_extractor;
  CRITICAL_SECTION        m_hDataInMutex;
  DataQueue               m_qDataIn;
  CRITICAL_SECTION        m_hDataOutMutex;
  DataQueue               m_qDataOut;

  /*
   *  Modules
   */
  IGameModule*            m_pCurrentModule;
  ModConsole              m_modConsole;
  ModForum                m_modForum;
  ModBank                 m_modBank;
  ModChat                 m_modChat;
};

#endif//_FCCONTROLLER_H_