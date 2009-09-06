#ifndef _FCSERVEROBJ_H_
#define _FCSERVEROBJ_H_

#include "Socket/ClientSocket.h"

class FCServerObj
{
public:
  FCServerObj(void);
  ~FCServerObj(void);

  // Latency function
  FCULONG GetLatency(FCBYTE pktType, FCSHORT msgID);

  // packet functions
  void RequestServerInfo();
  void Login(const char* username, const char* password);
  void RequestCharacterInfo();
  void SendCharacterSelection(size_t character_id);
  void RequestCharacterCreationParameters();
  void SendCharacterAssetRequest(size_t character_id);
	void SendCharacterItemsRequest(size_t character_id);
  void SendCharacterMissionsRequest(size_t character_id);
  void RequestDesktopOptions(size_t character_id);
	void RequestSoftwareActivate(FCULONG software_id);

  // console commands
  void RequestCharacterFileSystemInfo(FCULONG character_id);
	void RequestFileSystemList(const std::string& currentDir);
	void SendConsoleCommand(const std::string& currentDir, const std::string cmd);

  // forum commands
  void RequestForumCategories(FCULONG character_id);
	void RequestForumThreads(FCULONG category_id);
	void RequestForumThreadDetails(FCULONG category_id, FCULONG thread_id);
  void RequestForumThreadContentBlob(FCULONG category_id, FCULONG thread_id);
  void SendNewForumPost(FCULONG category_id, FCULONG thread_id, const char* pSubject, FCULONG msgLen, const char* pMessage);

  // bank commands
  void RequestBankConnect(FCULONG ticket = 0);
  void SendBankAccountCreateRequest(FCCHAR* password);
  void SendBankingPassword(FCCHAR* password, FCSHORT pwLen);
  void RequestBankAccountDetails(FCULONG ticket);

	// mission commands
	void SendMissionAccept(FCULONG mission_id);

  // software commands
  void SendInstallSoftwareRequest(FCSHORT portNum, FCULONG itemID);
  void SendUninstallSoftwareRequest(FCSHORT portNum);
  void SendNetworkPortEnableRequest(FCSHORT portNum, bool bEnable);
  void SendSoftwareStopped(FCULONG itemID);

  // chat commands
  void RequestChatRoomList(FCULONG character_id);

  void SetSocket(BaseSocket* pSock)                             { m_pSock = pSock; }

private:

  bool SendPacket(PEPacket& pkt);
  void SetLatencyAnchor(FCBYTE pktType, FCSHORT msgID);

  BaseSocket*             m_pSock;

  struct LatencyAnchor
  {
    FCBYTE pktType;
    FCSHORT msgID;
    FCULONG timestamp;
  }                       m_latencyAnchor;
};

#endif//_FCSERVEROBJ_H_