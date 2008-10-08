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
  strncpy(d.password, password, min( strlen(username), 64 )); 

  PEPacketHelper::CreatePacket(pkt, FCPKT_COMMAND, FCMSG_LOGIN, ST_Auth);
  PEPacketHelper::SetPacketData(pkt, &d, sizeof(d));

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