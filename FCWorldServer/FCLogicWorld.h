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
#ifndef _FCLOGICWORLD_H_
#define _FCLOGICWORLD_H_

#include <string>
#include <map>
#include <queue>
#include "../common/binstream.h"
#include "../common/threading.h"
#include "../common/fctypes.h"
#include "../common/PEPacket.h"
#include "../common/PacketExtractor.h"
#include "../common/ServiceLogicBase.h"
#include "../Clients/common/Socket/ClientSocket.h"

class FCLogicWorld : public ServiceLogicBase
                   , public IBaseSocketSink

{
  class RouterSocket : public BaseSocket
  {
  public:
    RouterSocket() {}
    ~RouterSocket() {}

    void SetServer(string server)             { m_server = server; }
    string GetServer()                        { return m_server; }
    void SetPort(short port)                  { m_port = port; }
    short GetPort()                           { return m_port; }
    void AddData(FCBYTE* pData, FCULONG len)
    {
      m_stream.Concat(pData, len);
    }
    CBinStream<FCBYTE, true>& GetDataStream() { return m_stream; }

  private:
    string              m_server;
    short               m_port;
    CBinStream<FCBYTE, true> m_stream;
  };

  typedef std::map< string, RouterSocket* > ServiceSocketMap;
  typedef std::queue<FCSOCKET> CQueuedSocketArray;

public:
  FCLogicWorld(void);
  ~FCLogicWorld(void);

  //
  // IServiceLogic implementation
  void Free();
  int Start();
  int Stop();

  //
  // IBaseSocketSink implementation
  void OnAccepted(BaseSocket* pSocket, int nErrorCode) {}
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

private:

  void RegisterServiceWithRouter(RouterSocket* pSock);

  void HandlePacket(PEPacket* pPkt, BaseSocket* pSocket);
  bool ConnectToRouters();
  void DisconnectFromRouters();
/*
  bool ConfigureDatabase();
  static void* thrdDBWorker(void* pData);
  void HandleCompletedDBJob(FCDBJob& job);
*/
  bool OnCommand(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnResponse(PEPacket* pPkt, BaseSocket* pSocket);
  bool OnError(PEPacket* pPkt, BaseSocket* pSocket);

  ServiceSocketMap m_mapRouters;
  PacketExtractor m_pktExtractor;
};

#endif//_FCLOGICWORLD_H_