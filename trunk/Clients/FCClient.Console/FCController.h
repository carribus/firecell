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
#include "../../common/fctypes.h"
#include "../../common/PacketExtractor.h"
#include "../../common/PEPacket.h"
#include "../common/Socket/ClientSocket.h"
#include "../common/FCServerObj.h"

using namespace std;

class FCController : IBaseSocketSink
{
  struct DataQueueItem
  {
    BaseSocket* pSocket;
    PEPacket* pPkt;
  };
  typedef queue<DataQueueItem> DataQueue;

public:
  FCController(void);
  virtual ~FCController(void);

  bool Initialise();
  FCINT Run();

protected:

  bool ConnectToServer(string server, short port);
  void Disconnect();

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

private:

  FCServerObj             m_server;
  BaseSocket              m_sock;
  bool                    m_bConnected;
  HANDLE                  m_hSockEvent;

  HANDLE                  m_hGameEvent;

  PacketExtractor         m_extractor;
  CRITICAL_SECTION        m_hDataInMutex;
  DataQueue               m_qDataIn;
  CRITICAL_SECTION        m_hDataOutMutex;
  DataQueue               m_qDataOut;
};

#endif//_FCCONTROLLER_H_