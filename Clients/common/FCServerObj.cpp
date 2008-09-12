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

  char* pData = NULL;
  size_t blockLen = 0;

  pkt.GetDataBlock(pData, blockLen);
  m_pSock->Send((FCBYTE*)pData, (int)blockLen);
}