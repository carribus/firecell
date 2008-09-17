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
#include <sstream>
#include "../common/protocol/fcprotocol.h"
#include "../common/PEPacket.h"
#include "../common/PEPacketHelper.h"
#include "FCLogicAuth.h"

FCLogicAuth::FCLogicAuth(void)
: m_bHasConsole(false)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicAuth::~FCLogicAuth(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicAuth::Start()
{
  // load the configuration
  if ( !LoadConfig("FCAuth.conf") )
  {
    if ( m_bHasConsole )
      printf("Failed to load FCauth.conf configuration file\n");
    return -1;
  }

  // kick off the database object

  // connect to the router(s) that we were configured to connect to
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicAuth::Stop()
{
  return 0;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  if ( !nErrorCode )
  {
    m_mapRouters[ pSock->GetServer() ] = pSock;
    RegisterServiceWithRouter(pSock);
  }
  else
  {
    if ( m_bHasConsole )
      printf("Failed to connect to router (%s:%ld)\n", pSock->GetServer().c_str(), pSock->GetPort());
  }
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
  RouterSocket* pSock = (RouterSocket*) pSocket;

  // temporary code - we probably need to attempt to reconnect to the dropped router
  m_mapRouters.erase( pSock->GetServer() );
  delete pSock;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::RegisterServiceWithRouter(RouterSocket* pSock)
{
  if ( !pSock )
    return;

  PEPacket pkt;

  __FCPKT_REGISTER_SERVER d;

  d.type = ST_Auth;

  PEPacketHelper::CreatePacket( pkt, FCPKT_COMMAND, FCMSG_REGISTER_SERVICE );
  PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_REGISTER_SERVER) );

  // send the packet
  size_t dataLen = 0;
  char* pData = NULL;

  pkt.GetDataBlock( pData, dataLen );
  pSock->Send( (FCBYTE*)pData, (FCUINT)dataLen );
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::LoadConfig(FCCSTR strFilename)
{
  if ( !strFilename )
    return false;

 return (m_config.Load(strFilename) == 0);
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::ConnectToRouters()
{
  bool bResult = false;
  int nRouterCount = 0;
  string strKey;
  string strValue, strServer;
  short port;
  INIFile::CSection* pSection = m_config.GetSection("Routers");

  if ( pSection )
  {
    for ( nRouterCount = 0; ;nRouterCount++ )
    {
      stringstream ss;
      ss << "Router" << nRouterCount;
      strKey = ss.str();
      strValue = pSection->GetValue(strKey);
      if ( !strValue.empty() )
      {
        strServer = strValue.substr(0, strValue.find(':'));
        port = atoi( strValue.substr( strValue.find(':')+1, strValue.length() ).c_str() );

        if ( m_bHasConsole )
          printf("Connecting to router (%s:%ld)\n", strServer.c_str(), port);

        RouterSocket* pSock = new RouterSocket;
        pSock->SetServer( strServer );
        pSock->SetPort( port );
        pSock->Subscribe(this);

        pSock->Create();
        pSock->Connect(strServer.c_str(), port);
      }
      else
        break;
    }
  }

  return bResult;
}