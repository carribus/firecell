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
#ifndef _FCLOGICAUTH_H_
#define _FCLOGICAUTH_H_

#include <string>
#include <map>
#include "../common/inifile.h"
#include "../common/PEPacket.h"
#include "../common/interfaces/IServiceLogic.h"
#include "../Clients/common/Socket/ClientSocket.h"

using namespace std;

class FCLogicAuth : public IServiceLogic
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

  private:
    string              m_server;
    short               m_port;
  };

  typedef std::map< string, RouterSocket* > ServiceSocketMap;


public:
  FCLogicAuth(void);
  ~FCLogicAuth(void);

  //
  // IServiceLogic implementation
  const char* GetName()                           { return "FireCell Authentication Service"; }
  void Free();
  int Start();
  int Stop();
  void HasConsole(bool bHasConsole)               { m_bHasConsole = bHasConsole; }

  //
  // IBaseSocketSink implementation
  void OnAccepted(BaseSocket* pSocket, int nErrorCode) {}
	void OnConnected(BaseSocket* pSocket, int nErrorCode);
	void OnDisconnected(BaseSocket* pSocket, int nErrorCode);
	void OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen);

private:

  void RegisterServiceWithRouter(RouterSocket* pSock);

  bool                LoadConfig(FCCSTR strFilename);
  bool                ConnectToRouters();

  bool                m_bHasConsole;
  INIFile             m_config;
  ServiceSocketMap    m_mapRouters;
};

#endif//_FCLOGICAUTH_H_