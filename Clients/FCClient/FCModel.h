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
#ifndef _FCMODEL_H_
#define _FCMODEL_H_

#include <queue>
#include <vector>
#include "../../common/fctypes.h"
#include "../../common/PacketExtractor.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IModelEventSink.h"
#include "PThreadMutex.h"
#include "FCModelEvent.h"
#include "Character.h"

using namespace std;

class FCModel : public IBaseSocketSink
{
	struct DataQueueItem
  {
    BaseSocket* pSocket;
    PEPacket* pPkt;
  };
  typedef queue<DataQueueItem> DataQueue;

public:

	/*
	 * Model states
	 */
	enum e_ModelState
	{
		None = 0,
		Loading,
		Connecting,
		Login,
		CharacterSelection,
		Playing,
		ShuttingDown
	};

	/*
	 *	Sub-states for each model state
	 */

	// e_ModelState::Loading
	enum e_ModelStateLoading
	{
    MS_Loading_None,
		MS_Loading_Text,
    MS_Loading_Graphics,
    MS_Loading_Sounds
	};

  // e_ModelState::Connecting
  enum e_ModelStateConnecting
  {
    MS_Connecting_None,
    MS_Connecting_Connecting,
    MS_Connecting_Connected,
    MS_Connecting_Retry,
    MS_Connecting_FinalFail
  };

	// e_ModelState::Login
	enum e_ModelStateLogin
	{
		MS_Login_None,
		MS_Login_LoginFailed,
		MS_Login_LoginFailed_AccountInUse,
		MS_Login_LoginSucceeded,
	};

	// e_ModelState::CharacterSelection
	enum e_ModelStateCharacterSelection
	{
		MS_CharacterSelection_None,
		MS_CharacterSelection_CharacterSelected,
		MS_CharacterSelection_NewCharacter,
		MS_CharacterSelection_DeleteCharacter,
		MS_CharacterSelection_Failed
	};

  // e_ModelState::ShuttingDown
  enum e_ModelStateShuttingDown
  {
    MS_ShuttingDown_None = 0,
    MS_ShuttingDown_InProgress
  };

  /*
   *  StateInfo structure that is passed along with the FCME_StateChange event
   */
	struct StateInfo
	{
		e_ModelState state;
		FCSHORT stateStep;
	};

  /* */
	FCModel(void);
	~FCModel(void);

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();
	void SetState(e_ModelState newState);
  void SetStateStep(FCSHORT stateStep);
	StateInfo GetState()														{ return m_state; }

	bool ProcessData();

	/*
	 * IBaseSocketSink implementation
	 */
	void OnAccepted(BaseSocket* pSocket, int nErrorCode);
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

	void StartLogin(wstring username, wstring password);
	void SelectCharacter(FCUINT characterID);

  vector<Character>& GetCharacters()               { return m_characters; }

private:

	bool LoadResources();

	bool ConnectToServer();

  void Shutdown();

	void FireEvent(e_FCEventType type, void* pData);

	void ProcessIncomingData();
	bool GetNextQueueItem(DataQueue& queue, DataQueueItem& dest);

	bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseServiceInfo(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseLogin(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseGetCharacters(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseSelectCharacter(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseCharacterAssetRequest(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseGetDesktopOptions(PEPacket* pPkt, BaseSocket* pSocket);

  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  PThreadMutex                    m_mutexSinks;
	vector<IModelEventSink*>				m_sinks;
	StateInfo												m_state;

	BaseSocket				m_sock;
	FCServerObj				m_server;
	PacketExtractor		m_extractor;
	bool							m_bConnected;
  FCSHORT           m_connectRetry;

	DataQueue					m_qDataIn;
	PThreadMutex			m_mutexDataIn;

  vector<Character> m_characters;
	Character*				m_pCharacter;					// selected character

	/*
	 * Desktop Options available to player
	 */
  struct DesktopOption
  {
    FCULONG optionID;
    FCUINT type;
    char name[32];
  };
  map<FCUINT, DesktopOption>   m_desktopOptions;
};

#endif//_FCMODEL_H_