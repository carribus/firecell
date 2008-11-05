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
    RegisterDBHandler(DBQ_LOGIN_CHARACTER, OnDBJob_LoginCharacter);
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
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCMSG_LOGIN:
    {
      bHandled = OnCommandLogin(pPkt, pRouter, clientSocket);
    }
    break;

  case  FCMSG_GETCHARACTERS:
    {
      bHandled = OnCommandGetCharacters(pPkt, pRouter, clientSocket);
    }
    break;

  case  FCMSG_SELECT_CHARACTER:
    {
      bHandled = OnCommandSelectCharacter(pPkt, pRouter, clientSocket);
    }
    break;

  case  FCSMSG_CLIENT_DISCONNECT:
    {
      bHandled = OnCommandClientDisconnect(pPkt, pRouter, clientSocket);
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommandLogin(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_LOGIN d;
  size_t dataLen = 0;
  DBJobContext* pCtx = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  pCtx = new DBJobContext;
  pCtx->pThis = this;
  pCtx->clientSocket = clientSocket;
  pCtx->pRouter = pSocket;

  GetDatabase().ExecuteJob(DBQ_LOAD_ACCOUNT, (void*)pCtx, d.username, d.password);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommandGetCharacters(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  Account* pAccount = GetAccountByClientSocket(clientSocket);
  DBJobContext* pCtx = NULL;

  if ( pAccount )
  {
    pCtx = new DBJobContext;
    pCtx->pThis = this;
    pCtx->clientSocket = clientSocket;
    pCtx->pRouter = pSocket;

    GetDatabase().ExecuteJob(DBQ_LOAD_CHARACTER_INFO, (void*)pCtx, pAccount->GetID());
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommandSelectCharacter(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  Account* pAccount = GetAccountByClientSocket(clientSocket);
  DBJobContext* pCtx = NULL;

  if ( pAccount )
  {
    __FCPKT_SELECT_CHARACTER d;
    size_t dataLen = 0;

    pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
    pPkt->GetField("data", (void*)&d, dataLen);

    pAccount->SetCurrentCharacterID(d.character_id);

    pCtx = new DBJobContext;
    pCtx->pThis = this;
    pCtx->clientSocket = clientSocket;
    pCtx->pRouter = pSocket;

    GetDatabase().ExecuteJob(DBQ_LOGIN_CHARACTER, (void*)pCtx, pAccount->GetID(), d.character_id, pAccount->GetID(), d.character_id);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnCommandClientDisconnect(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCSPKT_CLIENT_DISCONNECT d;
  size_t dataLen = 0;
  DBJobContext* pCtx = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  // log the account associated with this client socket out
  Account* pAccount = GetAccountByClientSocket(d.clientSocket);

  if ( pAccount )
  {
    // NOTE: This might be a bit brutal. Possibly investigate the potential of putting the account on a disconnect
    //       timer, at the end of which we log the account out. This will potentially make reconnects a little easier.
    pCtx = new DBJobContext;
    pCtx->pThis = this;
    pCtx->clientSocket = clientSocket;
    pCtx->pRouter = pSocket;

    GetDatabase().ExecuteJob(DBQ_LOGOUT_CHARACTER, (void*)pCtx, pAccount->GetID(), pAccount->GetCurrentCharacterID());

    AccountLogout( pAccount );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*)pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCMSG_REGISTER_SERVICE:
    {
      bHandled = OnResponseRegisterService(pPkt, pRouter, clientSocket);
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

bool FCLogicAuth::OnResponseRegisterService(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
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
        printf("Service registered with Router (%s:%ld)\n", pSocket->GetServer().c_str(), pSocket->GetPort());
    }
    else
    {
      // registration failed
      if ( HasConsole() )
        printf("Service failed to register with Router (%s:%ld)\n", pSocket->GetServer().c_str(), pSocket->GetPort());
    }

  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSocket = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSocket, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCSMSG_CHARACTER_LOGGEDIN:
    {
      bHandled = OnErrorSelectCharacter(pPkt, pRouter, clientSocket);
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicAuth::OnErrorSelectCharacter(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCSPKT_CHARACTER_LOGGEDIN_ERROR d;
  size_t dataLen = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  printf("Character Selection failed on World Service (charID=%ld)\n", d.character_id);

  return true;
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
    __FCPKT_CHARACTER_LIST d;
    
    memset(&d, 0, sizeof(__FCPKT_CHARACTER_LIST));
    // check if the account has any characters created against it
    if ( (d.numCharacters = resultSet.GetRowCount()) )
    {
      // there are characters available...
      for ( size_t i = 0; i < d.numCharacters; i++ )
      {
        d.characters[i].character_id = atoi( resultSet.GetValue("character_id", i).c_str() );
        strncpy( d.characters[i].name, resultSet.GetValue("name", i).c_str(), sizeof(d.characters[i].name)-1 );
        d.characters[i].xp = atoi( resultSet.GetValue("xp", i).c_str() );
        d.characters[i].level = atoi( resultSet.GetValue("level", i).c_str() );
        d.characters[i].fame_scale = atoi( resultSet.GetValue("fame_scale", i).c_str() );
        d.characters[i].country_id = atoi( resultSet.GetValue("country_id", i).c_str() );
        d.characters[i].city_id = atoi( resultSet.GetValue("city_id", i).c_str() );
      }
    }

    // send the character list
    PEPacketHelper::CreatePacket( pkt, FCPKT_RESPONSE, FCMSG_GETCHARACTERS );
    PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_CHARACTER_LIST) );
    pkt.SetFieldValue("target", &pCtx->clientSocket);
    pSock->Send(&pkt);
  }

  delete pCtx;
  pContext = NULL;
}

///////////////////////////////////////////////////////////////////////

void FCLogicAuth::OnDBJob_LoginCharacter(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicAuth* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  FCUINT accountID = atoi( resultSet.GetValue("account_id", 0).c_str() );
  FCUINT charID = atoi( resultSet.GetValue("character_id", 0).c_str() );
  bool bLoggedIn = atoi( resultSet.GetValue("is_logged_in", 0).c_str() ) ? true : false;
  Account* pAccount = pThis->GetAccountByClientSocket( pCtx->clientSocket );

  if ( pAccount )
  {
    if ( bLoggedIn )
    {
      // select the character onto the account
      pAccount->SetCurrentCharacterID( charID );
    }
  }

  if ( pSock )
  {
    // notify the player of the character selection result
    PEPacket pkt, pkt2;
/*
    __FCPKT_SELECT_CHARACTER_RESP d;

    d.bSuccess = bLoggedIn;
    d.character_id = charID;

    PEPacketHelper::CreatePacket( pkt, FCPKT_RESPONSE, FCMSG_SELECT_CHARACTER );
    PEPacketHelper::SetPacketData( pkt, (void*)&d, sizeof(__FCPKT_SELECT_CHARACTER_RESP) );
    pkt.SetFieldValue("target", &pCtx->clientSocket);
    pSock->Send(&pkt);
*/
    if ( bLoggedIn )
    {
      // notify the world service of the character selection
      __FCSPKT_CHARACTER_LOGGEDIN d2;
      ServiceType target = ST_World;

      d2.clientSocket = pCtx->clientSocket;
      d2.account_id = accountID;
      d2.character_id = charID;
      strncpy( d2.name, resultSet.GetValue("name", 0).c_str(), sizeof(d2.name) );
      d2.xp = atoi( resultSet.GetValue("xp", 0).c_str() );
      d2.level = atoi( resultSet.GetValue("level", 0).c_str() );
      d2.fame_scale = atoi( resultSet.GetValue("fame_scale", 0).c_str() );
      d2.country_id = atoi( resultSet.GetValue("country_id", 0).c_str() );
      d2.city_id = atoi( resultSet.GetValue("city_id", 0).c_str() );

      PEPacketHelper::CreatePacket( pkt2, FCPKT_COMMAND, FCSMSG_CHARACTER_LOGGEDIN );
      PEPacketHelper::SetPacketData( pkt2, (void*)&d2, sizeof(d2) );
      pkt2.SetFieldValue("target", (void*)&target);
      pSock->Send(&pkt2);
    }
  }  

  delete pCtx;
  pContext = NULL;
}
