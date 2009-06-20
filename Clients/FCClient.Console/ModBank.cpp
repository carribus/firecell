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