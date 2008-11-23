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
#include <conio.h>
#include <map>
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

bool FCController::Initialise(const string& username, const string& password)
{
  if ( !m_sock.Create() )
  {
    fprintf(stderr, "Failed to create socket\n");
    return false;
  }

  m_username = username;
  m_password = password;

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
  m_server.Login(m_username.c_str(), m_password.c_str());

  // game loop
  m_hGameEvent = CreateEvent(0, 0, 0, 0);
  while ( WaitForSingleObject(m_hGameEvent, 250) != WAIT_OBJECT_0 )
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

void FCController::Disconnect()
{
  printf("Disconnecting from server...\n");

  m_sock.Disconnect();
  m_bConnected = false;
  SetEvent(m_hGameEvent);
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
      bHandled = OnResponseServiceInfo(pPkt, pSocket);
    }
    break;

  case  FCMSG_LOGIN:
    {
      bHandled = OnResponseLogin(pPkt, pSocket);
    }
    break;

  case  FCMSG_GETCHARACTERS:
    {
      bHandled = OnResponseGetCharacters(pPkt, pSocket);
    }
    break;

  case  FCMSG_SELECT_CHARACTER:
    {
      bHandled = OnResponseSelectCharacter(pPkt, pSocket);
    }
    break;

  case  FCMSG_CHARACTER_ASSET_REQUEST:
    {
      bHandled = OnResponseCharacterAssetRequest(pPkt, pSocket);
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

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponseServiceInfo(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_INFO_SERVER d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);
  printf("\n\n\tv%d.%d\n" \
          "\t%ld Connections\n\n", d.verMajor, d.verMinor, d.connectionCountRouter);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponseLogin(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_LOGIN_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);
  switch ( d.loginStatus )
  {
  case  LoginFailed:
    printf("Login failed\n");
    Disconnect();
    break;

  case  LoginSuccess:
    printf("Login successful\n");
    m_server.RequestCharacterInfo();
    break;

  case  LoginAccountLoggedInAlready:
    printf("Login failed - Account is already logged in\n");
    Disconnect();
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponseGetCharacters(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CHARACTER_LIST d;
  size_t dataLen;
  map<FCUINT, size_t> KeyToCharacterMap;
  size_t characterSelected = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  if ( d.numCharacters <= 0 )
  {
    printf("No characters found for this account!\n");
  }
  else
  {
    printf("Characters available on account:\n\n");
    for ( FCUINT i = 0; i < d.numCharacters; i++ )
    {
      printf("[%c]\tID: %ld\n" \
              "\tName: %s\n" \
              "\tLevel: %ld\n" \
              "\tXP: %ld\n\n",
              65+i,
              d.characters[i].character_id,
              d.characters[i].name,
              d.characters[i].level,
              d.characters[i].xp);

      KeyToCharacterMap[65+i] = d.characters[i].character_id;
    }

    printf("Select the character you wish to play: ");
    do
    {
      int nKey = getch();
      map<FCUINT, size_t>::iterator it = KeyToCharacterMap.find(nKey);

      if ( it == KeyToCharacterMap.end() )
        continue;

      characterSelected = it->second;
    } while (characterSelected == 0);

    printf("Character %ld\n\n", characterSelected);
    m_server.SendCharacterSelection(characterSelected);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponseSelectCharacter(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_SELECT_CHARACTER_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  if ( d.status == CharacterSelectSucceeded )
  {
    printf("Character id:%ld selected\n", d.character_id);
    m_server.SendCharacterAssetRequest(d.character_id);
  }
  else
  {
    printf("Character selection failed (%ld)\n", d.status);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCController::OnResponseCharacterAssetRequest(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CHARACTER_ASSET_REQUEST_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  printf("\nIP Address: %ld.%ld.%ld.%ld\n", d.ip_address.a, d.ip_address.b, d.ip_address.c, d.ip_address.d);
  printf("Computer Specifications:\n\n");
  printf("\tName: %s\n\tHDD Size: %ld MB\n\tNetwork Speed: %ld MBits\n",
         d.computer.name, d.computer.hddSize, d.computer.networkSpeed);
  printf("\tProcessor:\n\t\tName: %s\n\t\tCore Count: %ld\n\t\tCore Speed: %ldMhz\n",
         d.computer.processor.name, d.computer.processor.core_count, d.computer.processor.core_speed);
  printf("\tOperating System:\n\t\tName: %s\n\t\tKernal: %s\n",
         d.computer.os.name, d.computer.os.kernel_name);
  printf("\tMemory Module:\n\t\tName: %s\n\t\tSize: %ldMB\n",
         d.computer.memory.name, d.computer.memory.mem_size);

  return true;
}
