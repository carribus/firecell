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
#include "../../common/protocol/fcprotocol.h"
#include "../../common/PEPacketHelper.h"
#include "FCController.h"

FCController::FCController(void)
: m_bConnected(false)
{
  InitializeCriticalSection(&m_hDataInMutex);
  InitializeCriticalSection(&m_hDataOutMutex);
}

///////////////////////////////////////////////////////////////////////

FCController::~FCController(void)
{
  DeleteCriticalSection(&m_hDataInMutex);
  DeleteCriticalSection(&m_hDataOutMutex);
}

///////////////////////////////////////////////////////////////////////

bool FCController::Initialise()
{
  if ( !m_sock.Create() )
  {
    fprintf(stderr, "Failed to create socket\n");
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

FCINT FCController::Run()
{
  string strServer = "127.0.0.1";
  short port = 6666;

  if ( !ConnectToServer(strServer, port) )
  {
    return -1;
  }

  m_server.SetSocket(&m_sock);
  m_extractor.Prepare( __FCPACKET_DEF );

  printf("Requesting Server Info: ");
  m_server.RequestServerInfo();
  m_server.Login("testuser", "testpass");

  // game loop
  m_hGameEvent = CreateEvent(0, 0, 0, 0);
  while ( WaitForSingleObject(m_hGameEvent, 250) )
  {
    ProcessIncomingData();
    ProcessOutgoingData();

    if ( !m_bConnected )
      break;
  }

  return 0;
}

///////////////////////////////////////////////////////////////////////

bool FCController::ConnectToServer(string server, short port)
{
  DWORD dwRes = 0;

  printf("Attempting to connect to %s:%ld....", server.c_str(), port);

  m_sock.Create();
  m_sock.Subscribe(this);

  m_hSockEvent = CreateEvent(0, 0, 0, 0);

  m_sock.Connect(server.c_str(), port);

  dwRes = WaitForSingleObject(m_hSockEvent, INFINITE);

  return m_bConnected;
}

///////////////////////////////////////////////////////////////////////

bool FCController::GetNextQueueItem(DataQueue& queue, DataQueueItem& dest)
{
  dest.pPkt = NULL;
  dest.pSocket = NULL;

  if ( queue.size() )
  {
    dest = queue.front();
    queue.pop();
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////

void FCController::ProcessIncomingData()
{
  FCBYTE type;
  DataQueueItem dqi;
  bool bContinue = false;

  EnterCriticalSection(&m_hDataInMutex);
  bContinue = GetNextQueueItem(m_qDataIn, dqi);
  LeaveCriticalSection(&m_hDataInMutex);

  while ( bContinue )
  {
    if ( dqi.pPkt->GetField("type", &type, sizeof(FCBYTE)) )
    {
      switch ( type )
      {
      case  FCPKT_COMMAND:
        OnCommand(dqi.pPkt, dqi.pSocket);
        break;

      case  FCPKT_RESPONSE:
        OnResponse(dqi.pPkt, dqi.pSocket);
        break;

      case  FCPKT_ERROR:
        OnError(dqi.pPkt, dqi.pSocket);
        break;
      }
    }

    EnterCriticalSection(&m_hDataInMutex);
    bContinue = GetNextQueueItem(m_qDataIn, dqi);
    LeaveCriticalSection(&m_hDataInMutex);
  }
}

///////////////////////////////////////////////////////////////////////

void FCController::ProcessOutgoingData()
{
}

///////////////////////////////////////////////////////////////////////

void FCController::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
  if ( !nErrorCode )
  {
    m_bConnected = true;
    printf("OK\n");
  }
  else
  {
    m_bConnected = false;
    printf("ERROR: %ld\n", nErrorCode);
  }

  SetEvent(m_hSockEvent);
}

///////////////////////////////////////////////////////////////////////

void FCController::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
  m_bConnected = false;

  printf("\nDisconnected from server (Code: %ld)\n", nErrorCode);
}

///////////////////////////////////////////////////////////////////////

void FCController::OnDataReceived(BaseSocket* pSocket, BYTE* pData, int nLen)
{
  PEPacket* pPkt = NULL;
  size_t offset = 0;
  DataQueueItem dqi;
  
  while ( (pPkt = m_extractor.Extract((const char*)pData, offset, (size_t)nLen)) )
  {
    dqi.pSocket = pSocket;
    dqi.pPkt = pPkt;
    EnterCriticalSection(&m_hDataInMutex);
    m_qDataIn.push(dqi);
    LeaveCriticalSection(&m_hDataInMutex);
  }
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));


  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  switch ( msgID )
  {
  case  FCMSG_INFO_SERVER:
    {
      size_t dataLen;
      __FCPKT_INFO_SERVER d;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", &d, dataLen);
      printf("\n\n\tv%d.%d\n" \
             "\t%ld Connections\n\n", d.verMajor, d.verMinor, d.connectionCountRouter);
      bHandled = true;
    }
    break;

  default:
    printf("Unknown Response packet received\n");
    pPkt->DebugDump();
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));


  return true;
}
