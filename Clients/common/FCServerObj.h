#ifndef _FCSERVEROBJ_H_
#define _FCSERVEROBJ_H_

#include "Socket/ClientSocket.h"

class FCServerObj
{
public:
  FCServerObj(void);
  ~FCServerObj(void);

  void RequestServerInfo();
  void Login(const char* username, const char* password);
  void RequestCharacterInfo();
  void SendCharacterSelection(size_t character_id);
  void RequestCharacterCreationParameters();
  void SendCharacterAssetRequest(size_t character_id);
  void RequestDesktopOptions(size_t character_id);
	void RequestDesktopOptionActivate(FCULONG option_id);

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

  void SetSocket(BaseSocket* pSock)                             { m_pSock = pSock; }

private:

  bool SendPacket(PEPacket& pkt);

  BaseSocket*             m_pSock;
};

#endif//_FCSERVEROBJ_H_