#include "../../common/protocol/fcprotocol.h"
#include "../../common/time/timelib.h"
#include "../../common/PEPacket.h"
#include "../../common/PEPacketHelper.h"
#include "FCServerObj.h"

FCServerObj::FCServerObj(void)
: m_pSock(NULL)
{
  memset(&m_latencyAnchor, 0, sizeof(LatencyAnchor));
}

///////////////////////////////////////////////////////////////////////

FCServerObj::~FCServerObj(void)
{
}

///////////////////////////////////////////////////////////////////////

FCULONG FCServerObj::GetLatency(FCBYTE pktType, FCSHORT msgID)
{
  if ( msgID == m_latencyAnchor.msgID &&
       pktType != m_latencyAnchor.pktType )
  {
    return (tl_getTime() - m_latencyAnchor.timestamp);
  }

  return 0;
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

void FCServerObj::RequestCharacterCreationParameters()
{
  PEPacket pkt;
  __FCPKT_GET_CHAR_CREATION_PARAMS d;

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_GET_CHAR_CREATION_PARAMS, ST_World);
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

void FCServerObj::RequestDesktopOptionActivate(FCULONG option_id)
{
	PEPacket pkt;
	__FCPKT_ACTIVATE_DESKTOP_OPTION d;

	d.optionID = option_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_ACTIVATE_DESKTOP_OPTION, ST_World);
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

void FCServerObj::RequestForumCategories(FCULONG character_id)
{
  PEPacket pkt;
  __FCPKT_FORUM_GET_CATEGORIES d;

  d.character_id = character_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_GET_CATEGORIES, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestForumThreads(FCULONG category_id)
{
	PEPacket pkt;
	__FCPKT_FORUM_GET_THREADS d;

	d.category_id = category_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_GET_THREADS, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestForumThreadDetails(FCULONG category_id, FCULONG thread_id)
{
	PEPacket pkt;
	__FCPKT_FORUM_GET_THREAD_DETAILS d;

	d.category_id = category_id;
	d.thread_id = thread_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_GET_THREAD_DETAILS, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::RequestForumThreadContentBlob(FCULONG category_id, FCULONG thread_id)
{
  PEPacket pkt;
  __FCPKT_FORUM_GET_THREAD_CONTENTBLOB d;

  d.category_id = category_id;
  d.thread_id = thread_id;
  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_GET_THREAD_CONTENTBLOB, ST_World);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

  SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SendNewForumPost(FCULONG category_id, FCULONG thread_id, const char* pSubject, FCULONG msgLen, const char* pMessage)
{
  PEPacket pkt;
  __FCPKT_FORUM_CREATE_NEW_THREAD* d;
  size_t pktLen = sizeof(__FCPKT_FORUM_CREATE_NEW_THREAD) + msgLen-1;

  d = (__FCPKT_FORUM_CREATE_NEW_THREAD*) new FCBYTE[ pktLen ];
  if ( d )
  {
    d->category_id = category_id;
		d->thread_id = thread_id;
    strncpy(d->title, pSubject, sizeof(d->title));
    d->contentLength = msgLen;
    strncpy(d->content, pMessage, d->contentLength);

    PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_FORUM_CREATE_NEW_THREAD, ST_World);
    PEPacketHelper::SetPacketData(pkt, (void*)d, pktLen);

    SendPacket(pkt);

    delete [] (FCBYTE*)d;
  }
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SendMissionAccept(FCULONG mission_id)
{
	PEPacket pkt;
	__FCPKT_MISSION_ACCEPT d;
	
	d.mission_id = mission_id;
	PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_MISSION_ACCEPT, ST_World);
	PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(d));

	SendPacket(pkt);
}

///////////////////////////////////////////////////////////////////////

bool FCServerObj::SendPacket(PEPacket& pkt)
{
  FCSHORT msgID = 0;
  FCBYTE pktType = 0;
  char* pData = NULL;
  size_t blockLen = 0;
  int nRet = 0;

  pkt.GetField("msg", &msgID, sizeof(FCSHORT));  
  pkt.GetField("type", &pktType, sizeof(FCBYTE));
  pkt.GetDataBlock(pData, blockLen);
  nRet = m_pSock->Send((FCBYTE*)pData, (int)blockLen);

  SetLatencyAnchor(pktType, msgID );

  return (nRet == blockLen);
}

///////////////////////////////////////////////////////////////////////

void FCServerObj::SetLatencyAnchor(FCBYTE pktType, FCSHORT msgID)
{
  m_latencyAnchor.pktType = pktType;
  m_latencyAnchor.msgID = msgID;
  m_latencyAnchor.timestamp = tl_getTime();
}