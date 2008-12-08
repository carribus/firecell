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
  void SendCharacterAssetRequest(size_t character_id);
  void RequestDesktopOptions(size_t character_id);

  // console commands
  void RequestCharacterFileSystemInfo(FCULONG character_id);
  void RequestFileSystemList(const string& currentDir);
  void SendConsoleCommand(const string& currentDir, const string cmd);

  // forum commands
  void RequestForumCategories(FCULONG character_id);

  void SetSocket(BaseSocket* pSock)                             { m_pSock = pSock; }

private:

  bool SendPacket(PEPacket& pkt);

  BaseSocket*             m_pSock;
};

#endif//_FCSERVEROBJ_H_