#ifndef _FCSERVEROBJ_H_
#define _FCSERVEROBJ_H_

#include "Socket/ClientSocket.h"

class FCServerObj
{
public:
  FCServerObj(void);
  ~FCServerObj(void);

  void RequestServerInfo();

  void SetSocket(BaseSocket* pSock)                             { m_pSock = pSock; }

private:

  BaseSocket*             m_pSock;
};

#endif//_FCSERVEROBJ_H_