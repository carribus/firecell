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
#include "../../common/PThreadMutex.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"
#include "IModelEventSink.h"
#include "FCModelEvent.h"
#include "Character.h"
#include "Country.h"
#include "MissionMgr.h"
#include "ItemMgr.h"

using namespace std;

class FCModel : public IBaseSocketSink
{
	struct DataQueueItem
  {
    enum DQIType
    {
      DQI_Disconnect,
      DQI_Connect,
      DQI_DataIn
    } type;
    BaseSocket* pSocket;
    PEPacket* pPkt;
  };
  typedef queue<DataQueueItem> DataQueue;

	FCModel(void);
	~FCModel(void);

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

	/*
	 *	Desktop Options available to the player
	 */
  struct DesktopOption
  {
    FCULONG optionID;
    FCUINT type;
    char name[32];
  };

	/*
	 *	FileSystem information
	 */
	struct FileSystemInfo
	{
		std::string currentDir;
		std::string dirSeperator;
		FCBYTE fsStyle;
	};

  /* static accessors */
  static FCModel& instance();
  static void destroy();

	void SubscribeToEvents(IModelEventSink* pSink);
	bool IsSubscribed(IModelEventSink* pSink);

	bool Initialise();
	void SetState(e_ModelState newState);
  void SetStateStep(FCSHORT stateStep);
	StateInfo GetState()														{ return m_state; }
  FCULONG GetLatency()                            { return m_latency; }

  Character* GetPlayer()                          { return m_pCharacter; }

	bool ProcessData();

	/*
	 * IBaseSocketSink implementation
	 */
	void OnAccepted(BaseSocket* pSocket, int nErrorCode);
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

	void StartLogin(wstring username, wstring password);
  /* Character selection/creation/deletion */
	void SelectCharacter(FCUINT characterID);
  void StartNewCharacterCreation();
  /* Desktop */
	void ActivateDesktopOption(FCULONG softwareID);
	/* Console methods */
	void ConsoleRefresh();
	void ConsoleCommandIssued(std::string cmd, std::string curdir);
  /* Forum methods */
  void ForumGetCategories();
	void ForumGetThreads(FCULONG category_id);
  void ForumCreateNewThread(FCULONG category_id, FCULONG thread_id, std::wstring& subject, std::wstring& message);
  void ForumGetThreadDetails(FCULONG category_id, FCULONG thread_id);
	/* Mission methods */
	void MissionAccept(FCULONG mission_id);
  /* Software methods */
  void InstallSoftware(FCSHORT portNum, FCULONG itemID);
  void UninstallSoftware(FCSHORT portNum);
  void EnableNetworkPort(FCSHORT portNum, bool bEnable);

  vector<Character>& GetCharacters()                    { return m_characters; }
  std::map<FCULONG, Country>& GetCountries()            { return m_countries; }
  std::map<FCUINT, DesktopOption> GetDesktopOptions()	  { return m_desktopOptions; }
	MissionMgr& GetMissionMgr()											      { return m_missionMgr; }
  ItemMgr& GetItemMgr()                                 { return m_itemMgr; }

private:

	bool LoadResources();

	bool ConnectToServer();

  void Shutdown();

	void FireEvent(e_FCEventType type, void* pData);

	void ProcessIncomingData();
	bool GetNextQueueItem(DataQueue& queue, DataQueueItem& dest);

	bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnCommandMissionComplete(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnCommandXPGained(PEPacket* pPkt, BaseSocket* pSocket);

  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseServiceInfo(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseLogin(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseGetCharacters(PEPacket* pPkt, BaseSocket* pSocket);  
		bool OnResponseSelectCharacter(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseCharacterCreationParams(PEPacket* pPkt, BaseSocket* pSocket);
      bool OnResponseCharacterCreationParams_Countries(PEPacket* pPkt, BaseSocket* pSocket);
      bool OnResponseCharacterCreationParams_Cities(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseCharacterAssetRequest(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseCharacterItemsRequest(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseCharacterMissionsRequest(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseGetDesktopOptions(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseActivateSoftware(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseConsoleGetFileSystemInfo(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseConsoleCommand(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseForumGetCategories(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseForumGetThreads(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseForumGetThreadDetails(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseForumGetThreadContentBlob(PEPacket* pPkt, BaseSocket* pSocket);
		bool OnResponseForumCreateNewThread(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseMissionAccepted(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseSoftwareInstall(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseSoftwareUninstall(PEPacket* pPkt, BaseSocket* pSocket);
    bool OnResponseNetworkPortEnable(PEPacket* pPkt, BaseSocket* pSocket);

  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

	std::string ParseMissionString(std::string source);

  /*
   *  Private Data Members
   */

  static FCModel*                 m_pThis;

  PThreadMutex                    m_mutexSinks;
	vector<IModelEventSink*>				m_sinks;
	StateInfo												m_state;

	BaseSocket				m_sock;
	FCServerObj				m_server;
	PacketExtractor		m_extractor;
	bool							m_bConnected;
  FCSHORT           m_connectRetry;
  FCULONG           m_latency;

	DataQueue					m_qDataIn;
	PThreadMutex			m_mutexDataIn;

  vector<Character> m_characters;
	Character*				m_pCharacter;					// selected character

  /*
   *  Geography information
   */
	std::map<FCULONG, Country>   m_countries;

	/*
	 * Desktop Options available to player
	 */
	std::map<FCUINT, DesktopOption>   m_desktopOptions;

	/*
	 *	Console specific stuff
	 */
	FileSystemInfo		m_fsInfo;

  /*
   *  Mission stuff
   */
  MissionMgr m_missionMgr;

	/*
	 *	Items
	 */
	ItemMgr m_itemMgr;
};

#endif//_FCMODEL_H_