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
#include <conio.h>
#include <iostream>
#include "../../common/protocol/fcprotocol.h"
#include "ModBank.h"

ModBank::ModBank(void)
: m_pSink(NULL)
, m_characterID(0)
, m_pServer(NULL)
, m_state(ModBank::Initialise)
, m_ticket(0)
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
        m_pServer->RequestBankConnect(m_ticket);
        m_state = WaitingForResponse;
      }
    }
    break;

  case  ModBank::Running:
    {
      printf("Banking Options\n");
      printf("---------------\n");
      printf("[1] Get Account Details\n");
      printf("[2] Transfer\n");
      printf("[0] Exit\n");
      printf("\nSelect option: ");

      int nOption = _getch();
      while ( nOption < (int)'0' || nOption > (int)'2' )
        nOption = _getch();
      printf("\n");

      nOption -= 48;
      switch ( nOption )
      {
      case  0:
        if ( m_pSink )
          m_pSink->CloseModule(this);
        break;

      case  1:    // Get Account Details
        {
          m_pServer->RequestBankAccountDetails(m_ticket);
        }
        break;

      case  2:    // Transfer
        {

        }
        break;
  
      default:
        break;
      }
    }
    break;

  case  ModBank::WaitingForResponse:
    break;

  case  ModBank::NoAccountExists:
    {
      printf("No bank account has been created for this character\n");
      printf("Do you want to create a bank account (Y/N) : ");
      int nOption = _getch();

      while ( !(nOption == 'Y' || nOption == 'y') && !(nOption == 'N' || nOption == 'n') )
        nOption = _getch();
      printf("\n");

      if ( nOption == 'Y' || nOption == 'y' )
      {
        m_state = ModBank::CreatingAccount;
      }
      else if ( nOption == 'N' || nOption == 'n' )
      {
        printf("Closing bank...\n");
				if ( m_pSink )
					m_pSink->CloseModule(this);
      }
    }
    break;

  case  ModBank::CreatingAccount:
    {
      FCCHAR buffer[33];
      printf("Account creation: Do you want a password for your account? (Y/N): ");
      int nOption = _getch();

      memset(buffer, 0, sizeof(buffer));

      while ( !(nOption == 'Y' || nOption == 'y') && !(nOption == 'N' || nOption == 'n') )
        nOption = _getch();
      printf("\n");

      if ( nOption == 'Y' || nOption == 'y' )
      {
        printf("Enter a password (max 32 characters): ");

        cin >> buffer;
      }

      m_pServer->SendBankAccountCreateRequest(buffer);
      m_state = ModBank::WaitingForResponse;
    }
    break;

  case  ModBank::NeedAuth:
    {
      FCCHAR buffer[32];
      printf("Authentication required\nEnter your bank password: ");

      cin >> buffer;

      m_pServer->SendBankingPassword(buffer, strlen(buffer));
      m_state = ModBank::WaitingForResponse;
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

  case  FCMSG_BANK_CREATE_ACCOUNT:
    bHandled = OnResponseBankCreateAccount(pPkt, pSocket);
    break;

  case  FCMSG_BANK_AUTHENTICATE:
    bHandled = OnResponseBankAuthenticate(pPkt, pSocket);
    break;

  case  FCMSG_BANK_GET_DETAILS:
    bHandled = OnResponseBankGetDetails(pPkt, pSocket);
    break;

  default:
    break;
  }

  if ( bHandled && m_state == ModBank::WaitingForResponse )
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

  switch ( d.status )
  {
  case  BS_OK:
    m_ticket = d.ticket;
    m_state = ModBank::Running;
    break;

  case  BS_NoAccountExists:
    m_state = ModBank::NoAccountExists;
    break;

  case  BS_AccountNeedAuth:
    m_state = ModBank::NeedAuth;
    break;

  default:
    printf("[BANK] FCMSG_BANK_CONNECT Response: Unknown BankStatus received\n");
    if ( m_pSink )
      m_pSink->CloseModule(this);
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModBank::OnResponseBankCreateAccount(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_BANK_CREATE_ACCOUNT_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  if ( d.bResult )
  {
    m_ticket = d.ticket;
    printf("[BANK] Account created successfully!\n");
  }
  else
    printf("[BANK] Account creation failed!\n");

  m_state = ModBank::Running;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModBank::OnResponseBankAuthenticate(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_BANK_AUTHENTICATE_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  if ( d.bResult )
  {
    m_ticket = d.ticket;
    m_state = ModBank::Running;
  }
  else
  {
    printf("Authentication failed!");
    m_state = ModBank::Initialise;
    if ( m_pSink )
      m_pSink->CloseModule(this);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool ModBank::OnResponseBankGetDetails(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_BANK_GET_DETAILS_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  printf("\nBalance  : %ld\n", d.balance);
  printf("Debt     : %ld\n", d.debt);
  printf("%% Rate   : %ld\n", d.interest_rate);
  printf("Secure   : %s\n\n", d.is_secure ? "Yes" : "No");

  return true;
}