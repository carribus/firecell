#include <stdio.h>
#include <iostream>
#include "../../common/protocol/fcprotocol.h"
#include "ModBank.h"

ModBank::ModBank(void)
: m_pSink(NULL)
, m_characterID(0)
, m_pServer(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ModBank::~ModBank(void)
{
}

///////////////////////////////////////////////////////////////////////

void ModBank::QueueForAction()
{
  switch ( m_state )
  {
  case  ModBank::Initialise:
    {
      if ( m_pServer )
      {
        m_pServer->RequestBankConnect(m_characterID);
        m_state = WaitingForResponse;
      }
    }
    break;

  case  ModBank::Running:
    break;

  case  ModBank::WaitingForResponse:
    break;

  case  ModBank::NeedAuth:
    {
      FCCHAR buffer[32];
      printf("Authentication required\nEnter your bank password: ");

      cin >> buffer;

      m_pServer->SendBankingPassword(buffer, strlen(buffer));
    }
    break;

  default:
    printf("[Bank] Unknown state reached (%ld)\n", m_state);
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool ModBank::OnResponse(FCSHORT msgID, PEPacket* pPkt, BaseSocket* pSocket)
{
  bool bHandled = false;

  switch ( msgID )
  {
  case  FCMSG_BANK_CONNECT:
    bHandled = OnResponseBankConnect(pPkt, pSocket);
    break;

  default:
    break;
  }

  if ( bHandled )
    m_state = ModBank::Running;

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool ModBank::OnResponseBankConnect(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_BANK_CONNECT_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  m_state = Running;

  return true;
}