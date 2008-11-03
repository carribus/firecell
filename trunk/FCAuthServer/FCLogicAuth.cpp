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
: ServiceLogicBase("FireCell Authentication Service", false)
{
  pthread_mutex_init(&m_mutexLoggedInAccounts, NULL);
  pthread_mutex_init(&m_mutexSocketToAccount, NULL);
}

///////////////////////////////////////////////////////////////////////

FCLogicAuth::~FCLogicAuth(void)
{
  pthread_mutex_destroy(&m_mutexLoggedInAccounts);
  pthread_mutex_destroy(&m_mutexSocketToAccount);
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
  string strEngine, strServer, strDBName, strUser, strPass;

  // load the configuration
  if ( !LoadConfig("FCAuth.conf") )
  {
    if ( HasConsole() )
      printf("Failed to load FCauth.conf configuration file\n");
    return -1;
  }

  // initialise the packet extractor
  m_pktExtractor.Prepare( __FCPACKET_DEF );

  // kick off the database object
  if ( HasConsole() )
    printf("Setting up database connection...\n");

  if ( LoadDBSettingsFromConfig(strEngine, strServer, strDBName, strUser, strPass) )
  {
    if ( !ConfigureDatabase(strEngine, strServer, strDBName, strUser, strPass) )
    {
      return -1;
    }

    // register the DB Job Handlers
    RegisterDBHandler(DBQ_LOAD_ACCOUNT, OnDBJob_LoadAccount);
    RegisterDBHandler(DBQ_LOAD_CHARACTER_INFO, OnDBJob_LoadCharacterInfo);
  }

  // connect to the router(s) that we were configured to connect to
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicAuth::Stop()
{
  DisconnectFromRouters();
  return 0;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::IsAccountLoggedIn(int nAccountID)
{
  AccountMap::iterator it = m_mapLoggedInAccounts.find(nAccountID);

  return (it == m_mapLoggedInAccounts.end());
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::AccountLogin(int nAccountID, string strAccountName, int nAccountType, FCUINT clientSocket)
{
  Account* pAccount = NULL;
  AccountMap::iterator it = m_mapLoggedInAccounts.find(nAccountID);

  // if the account is not currently in the map, then we can simply add it
  if ( it == m_mapLoggedInAccounts.end() )      
  {
    pAccount = new Account;
    pAccount->SetID(nAccountID);
    pAccount->SetName(strAccountName);
    pAccount->SetType(nAccountType);
    pAccount->SetClientSocket(clientSocket);

    // map the account ID to the account
    pthread_mutex_lock(&m_mutexLoggedInAccounts);
    m_mapLoggedInAccounts[nAccountID] = pAccount;
    pthread_mutex_unlock(&m_mutexLoggedInAccounts);

    // map the router's client socket to the account
    pthread_mutex_lock(&m_mutexSocketToAccount);
    m_mapSocketToAccount[clientSocket] = pAccount;
    pthread_mutex_unlock(&m_mutexSocketToAccount);
  }
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::AccountLogout(Account* pAccount)
{
  if ( !pAccount )
    return;

  pthread_mutex_lock(&m_mutexLoggedInAccounts);
  m_mapLoggedInAccounts.erase( pAccount->GetID() );
  pthread_mutex_unlock(&m_mutexLoggedInAccounts);

  pthread_mutex_lock(&m_mutexSocketToAccount);
  m_mapSocketToAccount.erase( pAccount->GetClientSocket() );
  pthread_mutex_unlock(&m_mutexSocketToAccount);

  delete pAccount;
}

///////////////////////////////////////////////////////////////////////

Account* FCLogicAuth::GetAccountByClientSocket(FCUINT clientSocket)
{
  Account* pAccount = NULL;
  ClientSocketAccountMap::iterator it = m_mapSocketToAccount.find(clientSocket);

  if ( it != m_mapSocketToAccount.end() )
    pAccount = it->second;

  return pAccount;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSock = 0;
  DBJobContext* pCtx = NULL;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSock, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCMSG_LOGIN:
    {
      __FCPKT_LOGIN d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      pCtx = new DBJobContext;
      pCtx->pThis = this;
      pCtx->clientSocket = clientSock;
      pCtx->pRouter = pRouter;

      GetDatabase().ExecuteJob(DBQ_LOAD_ACCOUNT, (void*)pCtx, d.username, d.password);

      bHandled = true;
    }
    break;

  case  FCSMSG_CLIENT_DISCONNECT:
    {
      __FCSPKT_CLIENT_DISCONNECT d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      // log the account associated with this client socket out
      Account* pAccount = GetAccountByClientSocket(d.clientSocket);

      if ( pAccount )
      {
        // NOTE: This might be a bit brutal. Possibly investigate the potential of putting the account on a disconnect
        //       timer, at the end of which we log the account out. This will potentially make reconnects a little easier.
        AccountLogout( pAccount );
      }
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*)pSocket;
  FCSHORT msgID = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  switch ( msgID )
  {
  case  FCMSG_REGISTER_SERVICE:
    {
      __FCPKT_REGISTER_SERVER d;
      size_t dataLen = 0;

      pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
      pPkt->GetField("data", (void*)&d, dataLen);

      if ( d.type == ST_Auth )
      {
        if ( d.status )
        {
          // registration succeeded
          if ( HasConsole() )
            printf("Service registered with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }
        else
        {
          // registration failed
          if ( HasConsole() )
            printf("Service failed to register with Router (%s:%ld)\n", pRouter->GetServer().c_str(), pRouter->GetPort());
        }

      }
    }
    break;

  default:
    if ( HasConsole() )
      printf("Unknown Message Received (%ld)\n", msgID);
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDBJob_LoadAccount(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicAuth* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  if ( pSock )
  {
    // send the result of the login attempt to the client
    PEPacket pkt;
    string strAccID;
    __FCPKT_LOGIN_RESP d;

    int nAccountID = atoi( resultSet.GetValue( "account_id", 0 ).c_str() );
    string strAccountName = resultSet.GetValue( "acc_name", 0 );
    int nAccountType = atoi( resultSet.GetValue( "acc_type", 0 ).c_str() );

    if ( nAccountID > 0 )
    {
      // check if this account is logged in already (could be due to a disconnect that has not been handled yet)
      if ( !pThis->IsAccountLoggedIn(nAccountID) )
      {
        d.loginStatus = LoginAccountLoggedInAlready;
      }
      else
      {
        d.loginStatus = LoginSuccess;
      }
    }
    else
    {
      d.loginStatus = LoginFailed;
    }

    // send the login result
    PEPacketHelper::CreatePacket( pkt, FCPKT_RESPONSE, FCMSG_LOGIN );
    PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_LOGIN_RESP) );
    pkt.SetFieldValue("target", &pCtx->clientSocket);
    pSock->Send(&pkt);

    // if the login was successful, register the player as being logged in
    if ( d.loginStatus == LoginSuccess )
    {
      pThis->AccountLogin(nAccountID, strAccountName, nAccountType, pCtx->clientSocket);
    }

  }

  delete pCtx;
  pContext = NULL;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDBJob_LoadCharacterInfo(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  RouterSocket* pSock = pCtx->pRouter;

  if ( pSock )
  {
    PEPacket pkt;
    
    // check if the account has any characters created against it
    if ( resultSet.GetRowCount() )
    {
      // there are characters available...

    }
  }

  delete pCtx;
  pContext = NULL;
}