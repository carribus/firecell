#include "../../common/protocol/fcprotocol.h"
#include "../../common/PEPacket.h"
#include "../../common/PEPacketHelper.h"
#include "FCServerObj.h"

FCServerObj::FCServerObj(void)
: m_pSock(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCServerObj::~FCServerObj(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestServerInfo()
{
  PEPacket pkt;
  int nVal;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_INFO_SERVER);
  nVal = 0;
  PEPacketHelper::SetPacketData(pkt, &nVal, 1);
  
  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::Login(const char* username, const char* password)
{
  PEPacket pkt;
  __FCPKT_LOGIN d;

  memset(&d, 0, sizeof(d));
  strncpy(d.username, username, min( strlen(username), 64 ));
  strncpy(d.password, password, min( strlen(password), 64 )); 

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_LOGIN, ST_Auth);
  PEPacketHelper::SetPacketData(pkt, &d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestCharacterInfo()
{
  PEPacket pkt;
  int nVal;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_GETCHARACTERS, ST_Auth);
  nVal = 0;
  PEPacketHelper::SetPacketData(pkt, &nVal, 1);
  
  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SendCharacterSelection(size_t character_id)
{
  PEPacket pkt;
  __FCPKT_SELECT_CHARACTER d;

  d.character_id = (FCUINT)character_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_SELECT_CHARACTER, ST_Auth);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SendCharacterAssetRequest(size_t character_id)
{
  PEPacket pkt;
  __FCPKT_CHARACTER_ASSET_REQUEST d;

  d.character_id = (FCUINT)character_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_CHARACTER_ASSET_REQUEST, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestDesktopOptions(size_t character_id)
{
  PEPacket pkt;
  __FCPKT_GET_DESKTOP_OPTIONS d;

  d.character_id = (FCUINT)character_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_GET_DESKTOP_OPTIONS, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestCharacterFileSystemInfo(FCULONG character_id)
{
  PEPacket pkt;
  __FCPKT_CON_GET_FS_INFO d;

  d.character_id = character_id;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_CON_GET_FS_INFO, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestFileSystemList(const string& currentDir)
{
  PEPacket pkt;
  __FCPKT_CON_GET_FILE_LIST d;

  strncpy(d.currentDir, currentDir.c_str(), sizeof(d.currentDir));
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_CON_GET_FILE_LIST, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SendConsoleCommand(const string& currentDir, const string cmd)
{
  PEPacket pkt;
  __FCPKT_CON_COMMAND d;

  strncpy(d.currentDir, currentDir.c_str(), sizeof(d.currentDir));
  strncpy(d.command, cmd.c_str(), sizeof(d.command));
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_CON_COMMAND, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestForumThreads(FCULONG character_id)
{
  PEPacket pkt;
  __FCPKT_FORUM_GET_THREADS d;

  d.character_id = character_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_GET_THREADS, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

bool FCServerObj::SendPacket(PEPacket& pkt)
{
  char* pData = NULL;
  size_t blockLen = 0;
  int nRet = 0;

  pkt.GetDataBlock(pData, blockLen);
  nRet = m_pSock->Send((FCBYTE*)pData, (int)blockLen);

  return (nRet == blockLen);
}