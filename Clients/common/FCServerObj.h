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

  void SetSocket(BaseSocket* pSock)                             { m_pSock = pSock; }

private:

  bool SendPacket(PEPacket& pkt);

  BaseSocket*             m_pSock;
};

#endif//_FCSERVEROBJ_H_