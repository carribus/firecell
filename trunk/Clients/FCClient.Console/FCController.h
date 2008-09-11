#ifndef _FCCONTROLLER_H_
#define _FCCONTROLLER_H_

#include <string>
#include "../../common/fctypes.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"

using namespace std;

class FCController : IBaseSocketSink
{
public:
  FCController(void);
  virtual ~FCController(void);

  bool Initialise();
  FCINT Run();

protected:

  bool ConnectToServer(string server, short port);

  void CreateBasePacket(PEPacket& pkt, FCBYTE type);
  void RequestServerInfo();

  //
  // IBaseSocketSink implementation
	void OnAccepted(BaseSocket* pSocket, int nErrorCode);
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, BYTE* pData, int nLen);

private:

  BaseSocket              m_sock;
  bool                    m_bConnected;
  HANDLE                  m_hSockEvent;

  HANDLE                  m_hGameEvent;
};

#endif//_FCCONTROLLER_H_