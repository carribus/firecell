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
#include "../common/PEPacketHelper.h"
#include "EventSystem.h"
#include "Event.h"
#include "EventWithDisposableData.h"
#include "FCLogicWorld.h"

#define DBQ_LOAD_OBJECT_DATA    "load_object_data"

FCLogicWorld::FCLogicWorld()
: ServiceLogicBase("FireCell World Service", false)
{
  pthread_cond_init(&m_condSync, NULL);
  pthread_mutex_init(&m_mutexSync, NULL);
}

///////////////////////////////////////////////////////////////////////

FCLogicWorld::~FCLogicWorld(void)
{
  pthread_cond_destroy(&m_condSync);
  pthread_mutex_destroy(&m_mutexSync);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////
#include <sstream>
int FCLogicWorld::Start()
{
  string strEngine, strServer, strDBName, strUser, strPass;

  stringstream ss;

  // load the configuration
  if ( !LoadConfig("FCWorld.conf") )
  {
    if ( HasConsole() )
      printf("Failed to load FCWorld.conf configuration file\n");
    return -1;
  }

  INIFile::CSection* pSection = m_config.GetSection("Paths");
  if ( pSection )
  {
    m_pathFileSystems = pSection->GetValue("Filesystems");
  }

  // seed the RNG
  srand( (unsigned int)time(NULL) );

  // initialise the packet extractor
  m_pktExtractor.Prepare( __FCPACKET_DEF );

  /*
   *  Initialise the Database object
   */
  if ( HasConsole() )
    printf("Setting up database connection...\n");

  if ( LoadDBSettingsFromConfig(strEngine, strServer, strDBName, strUser, strPass) )
  {
    if ( !ConfigureDatabase(strEngine, strServer, strDBName, strUser, strPass) )
    {
      return -1;
    }

    // register the db handlers
    RegisterDBHandler(DBQ_LOAD_ITEM_TYPES, OnDBJob_LoadItemTypes);
    RegisterDBHandler(DBQ_LOAD_ITEM_DEFS, OnDBJob_LoadItemDefs);
    RegisterDBHandler(DBQ_LOAD_OBJECT_DATA, OnDBJob_LoadObjectData);
    RegisterDBHandler(DBQ_LOAD_CHARACTER_COMPUTER, OnDBJob_LoadCharacterComputer);
    RegisterDBHandler(DBQ_LOAD_WORLD_GEOGRAPHY, OnDBJob_LoadWorldGeography);
    RegisterDBHandler(DBQ_LOAD_COMPANIES, OnDBJob_LoadCompanies);
    RegisterDBHandler(DBQ_LOAD_COMPANY_COMPUTERS, OnDBJob_LoadCompanyComputers);
    RegisterDBHandler(DBQ_LOAD_MISSIONS, OnDBJob_LoadMissions);

    // Start the Item loading...
    if ( HasConsole() )
      printf("Loading Item Types...\n");

    DBJobContext* pCtx = new DBJobContext;
    pCtx->pThis = this;
    GetDatabase().ExecuteJob(DBQ_LOAD_ITEM_TYPES, (void*)pCtx);

    // we want to pause here until the item data required data is loaded
    pthread_mutex_lock(&m_mutexSync);
    pthread_cond_wait(&m_condSync, &m_mutexSync);
    pthread_mutex_unlock(&m_mutexSync);

    LoadWorldData();
  }

  /*
   *  Configure the Event System and set up dependencies
   */
  ConfigureEventSystem();
  m_playerMgr.SetEventSystem( EventSystem::GetInstance() );

  /*
   *  Connect to the router(s) that we were configured to connect to
   */
  ConnectToRouters();

  return 0;
}

///////////////////////////////////////////////////////////////////////

int FCLogicWorld::Stop()
{
  m_playerMgr.RemoveAllPlayers();
  EventSystem::Shutdown();
  DisconnectFromRouters();
  return 0;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::ConfigureEventSystem()
{
  EventSystem* events = EventSystem::GetInstance();
  FCDatabase& db = GetDatabase();

  events->Start();
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::LoadWorldData()
{
  DBJobContext* pCtx = new DBJobContext;
  pCtx->pThis = this;
  GetDatabase().ExecuteJob(DBQ_LOAD_WORLD_GEOGRAPHY, (void*)pCtx);

  // we want to pause here until the item data required data is loaded
  pthread_mutex_lock(&m_mutexSync);
  pthread_cond_wait(&m_condSync, &m_mutexSync);
  pthread_mutex_unlock(&m_mutexSync);

  pCtx = new DBJobContext;
  pCtx->pThis = this;
  GetDatabase().ExecuteJob(DBQ_LOAD_MISSIONS, (void*)pCtx);

  // we want to pause here until the item data required data is loaded
  pthread_mutex_lock(&m_mutexSync);
  pthread_cond_wait(&m_condSync, &m_mutexSync);
  pthread_mutex_unlock(&m_mutexSync);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendCharacterLoginStatus(FCULONG accountID, FCULONG characterID, e_SelectCharacterStatus status, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket pkt;
  __FCPKT_SELECT_CHARACTER_RESP d;

  // check if the action failed...
  if ( status != CharacterSelectSucceeded )
  {
    ServiceType target = ST_Auth;
    __FCSPKT_CHARACTER_LOGGEDIN_ERROR dError;

    dError.account_id = accountID;
    dError.character_id = characterID;
    dError.status = status;

    PEPacketHelper::CreatePacket(pkt, FCPKT_ERROR, FCSMSG_CHARACTER_LOGGEDIN, ST_Auth);
    PEPacketHelper::SetPacketData(pkt, (void*)&dError, sizeof(dError));

    pRouter->Send(&pkt);

    pkt.Empty();
  }

  d.character_id = characterID;
  d.status = status;

  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_SELECT_CHARACTER, ST_None);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(__FCPKT_SELECT_CHARACTER_RESP));

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendCharacterAssetResponse(Player* pPlayer, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPlayer )
    return;

  PEPacket pkt;
  __FCPKT_CHARACTER_ASSET_REQUEST_RESP d;
  Computer& comp = pPlayer->GetComputer();

  // ip address
  pPlayer->GetIP().GetIP( d.ip_address.a, d.ip_address.b, d.ip_address.c, d.ip_address.d );

  // computer data
  d.computer.id = comp.GetID();
  strncpy( d.computer.name, comp.GetName().c_str(), sizeof(d.computer.name)-1 );
  d.computer.hddSize = comp.GetTotalHDD();
  d.computer.networkSpeed = comp.GetNetworkSpeed();

  // processor data
  strncpy( d.computer.processor.name, comp.GetProcessor().GetName().c_str(), sizeof(d.computer.processor.name)-1 );
  d.computer.processor.item_id = comp.GetProcessor().GetID();
  d.computer.processor.itemtype_id = comp.GetProcessor().GetTypeID();
  d.computer.processor.min_level = comp.GetProcessor().GetMinLevel();
  d.computer.processor.max_level = comp.GetProcessor().GetMaxLevel();
  d.computer.processor.npc_value = comp.GetProcessor().GetNPCValue();
  d.computer.processor.core_count = comp.GetProcessor().GetCoreCount();
  d.computer.processor.core_speed = comp.GetProcessor().GetCoreSpeed();

  // operating system data
  strncpy( d.computer.os.name, comp.GetOS().GetName().c_str(), sizeof(d.computer.os.name)-1 );
  d.computer.os.item_id = comp.GetOS().GetID();
  d.computer.os.itemtype_id = comp.GetOS().GetTypeID();
  d.computer.os.min_level = comp.GetOS().GetMinLevel();
  d.computer.os.max_level = comp.GetOS().GetMaxLevel();
  d.computer.os.npc_value = comp.GetOS().GetNPCValue();
  d.computer.os.kernel_id = comp.GetOS().GetKernelID();
  strncpy( d.computer.os.kernel_name, comp.GetOS().GetKernelName().c_str(), sizeof( d.computer.os.kernel_name )-1 );

  // memory data
  strncpy( d.computer.memory.name, comp.GetMemory().GetName().c_str(), sizeof(d.computer.memory.name)-1 );
  d.computer.memory.item_id = comp.GetMemory().GetID();
  d.computer.memory.itemtype_id = comp.GetMemory().GetTypeID();
  d.computer.memory.min_level = comp.GetMemory().GetMinLevel();
  d.computer.memory.max_level = comp.GetMemory().GetMaxLevel();
  d.computer.memory.npc_value = comp.GetMemory().GetNPCValue();
  d.computer.memory.mem_size = comp.GetMemory().GetMemorySize();

  // send the packet
  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_CHARACTER_ASSET_REQUEST, ST_None);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(__FCPKT_CHARACTER_ASSET_REQUEST_RESP));

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendCharacterDesktopOptions(Player* pPlayer, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPlayer )
    return;

  PEPacket pkt;
  __FCPKT_GET_DESKTOP_OPTIONS_RESP* d = NULL;
  int nPktLen = 0;
  int numOptions = 7;
  char* OptionNames[] =
  {
    "Hacking Forum",
    "News Site",
    "Email",
    "Console",
    "Bank",
    "Chat",
    "Hacking Tools"
  };

  nPktLen = sizeof(__FCPKT_GET_DESKTOP_OPTIONS_RESP) + ( (numOptions-1)*sizeof(__FCPKT_GET_DESKTOP_OPTIONS_RESP::DesktopOptions) );
  d = (__FCPKT_GET_DESKTOP_OPTIONS_RESP*) new FCBYTE[ nPktLen ];
  memset(d, 0, nPktLen);
  d->numOptions = numOptions;

  for ( int i = 0; i < numOptions; i++ )
  {
    d->Options[i].optionID = i+1;
    d->Options[i].type = (DesktopOptionType)(i+1);
    strncpy( d->Options[i].name, OptionNames[i], sizeof(d->Options[i].name) );
  }

  // send the packet
  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_GET_DESKTOP_OPTIONS, ST_None);
  PEPacketHelper::SetPacketData(pkt, 
                                (void*)d, 
                                nPktLen);

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);

  // clear the data portion
  delete [] d;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendConsoleFileSystemInfo(FileSystem& fs, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket pkt;
  __FCPKT_CON_GET_FS_INFO_RESP d;

  d.fsStyle = fs.GetFSStyle();
  strncpy(d.dirSeperator, fs.GetDirSeperator().c_str(), sizeof(d.dirSeperator));
  strncpy(d.currentDir, fs.GetCurrentPathName().c_str(), sizeof(d.currentDir));

  // send the packet
  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_CON_GET_FS_INFO, ST_None);
  PEPacketHelper::SetPacketData(pkt, (void*)&d, sizeof(__FCPKT_CON_GET_FS_INFO_RESP));

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendConsoleFileList(string currentDir, vector<FileSystem::File> files, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket pkt;
  __FCPKT_CON_GET_FILE_LIST_RESP* d = NULL;
  size_t numFiles = files.size();
  size_t nPktLen = sizeof(__FCPKT_CON_GET_FILE_LIST_RESP) + ( (numFiles-1)*sizeof(__FCPKT_CON_GET_FILE_LIST_RESP::_files) );
  FileSystem::File f;

  d = (__FCPKT_CON_GET_FILE_LIST_RESP*) new FCBYTE[ nPktLen ];
  memset(d, 0, nPktLen);
  d->numFiles = (FCULONG)numFiles;

  for ( size_t i = 0; i < numFiles; i++ )
  {
    f = files[i];
    strncpy( d->Files[i].filename, f.filename.c_str(), sizeof(d->Files[i].filename) );
    d->Files[i].is_mutable = f.is_mutable;
    d->Files[i].is_dir = (f.filetype == FileSystem::File::FT_Directory);
  }

  // send the packet
  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_CON_GET_FILE_LIST, ST_None);
  PEPacketHelper::SetPacketData(pkt, 
                                (void*)d, 
                                nPktLen);

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::SendConsoleCommandResult(Player* pPlayer, string result, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket pkt;
  __FCPKT_CON_COMMAND_RESP* d = NULL;
  size_t len = result.length();
  size_t pktLen = sizeof(__FCPKT_CON_COMMAND_RESP) + ( len );
  FileSystem& fs = pPlayer->GetComputer().GetFileSystem();

  d = (__FCPKT_CON_COMMAND_RESP*) new FCBYTE[ pktLen ];
  memset(d, 0, pktLen);
  d->len = (FCINT)len;
  strncpy(d->currentDir, fs.GetCurrentPathName().c_str(), sizeof(d->currentDir)-1);

  memcpy(d->result, result.c_str(), len);

  // send the packet
  PEPacketHelper::CreatePacket(pkt, FCPKT_RESPONSE, FCMSG_CON_COMMAND, ST_None);
  PEPacketHelper::SetPacketData(pkt, 
                                (void*)d, 
                                pktLen);

  // send notification to Client
  pkt.SetFieldValue("target", (void*)&clientSocket);
  pRouter->Send(&pkt);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  RouterSocket* pRouter = (RouterSocket*) pSocket;
  FCSHORT msgID = 0;
  FCSOCKET clientSock = 0;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  pPkt->GetField("target",  &clientSock, sizeof(FCSOCKET));

  switch ( msgID )
  {
  case  FCSMSG_CHARACTER_LOGGEDIN:
    {
      bHandled = OnCommandCharacterLoggedIn(pPkt, pRouter, clientSock);
    }
    break;

  case  FCMSG_CHARACTER_ASSET_REQUEST:
    {
      bHandled = OnCommandCharacterAssetRequest(pPkt, pRouter, clientSock);
    }
    break;

  case  FCMSG_GET_DESKTOP_OPTIONS:
    {
      bHandled = OnCommandGetDesktopOptions(pPkt, pRouter, clientSock);
    }
    break;

  /*
   *  Console Module Messages
   */
  case  FCMSG_CON_GET_FS_INFO:
    {
      bHandled = OnCommandConsoleGetFSInfo(pPkt, pRouter, clientSock);
    }
    break;

  case  FCMSG_CON_GET_FILE_LIST:
    {
      bHandled = OnCommandConsoleGetFileList(pPkt, pRouter, clientSock);
    }
    break;

  case  FCMSG_CON_COMMAND:
    {
      bHandled = OnCommandConsoleCommand(pPkt, pRouter, clientSock);
    }
    break;

  /*
   *  Forum Module Messages
   */
    {
      bHandled = OnCommandForumGetThreads(pPkt, pRouter, clientSock);
    }

  /*
   * Inter-service Messages
   */
  case  FCSMSG_CLIENT_DISCONNECT:
    {
      bHandled = OnCommandClientDisconnect(pPkt, pRouter, clientSock);
    }
    break;

  default:
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandCharacterLoggedIn(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCSPKT_CHARACTER_LOGGEDIN d;
  size_t dataLen = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  // create the player's character
  Player* pPlayer = m_playerMgr.CreatePlayer(d.account_id, d.character_id, d.name, d.xp, d.level, d.fame_scale, d.country_id, d.city_id, d.clientSocket);

  // if we fail to create or locate an existing player object, then something went terribly wrong...
  // we need to notify the player of the problem, as well as the auth service
  if ( !pPlayer )
  {
    SendCharacterLoginStatus(d.account_id, d.character_id, CharacterSelectFailedInWorld, pRouter, d.clientSocket);
  }
  else
  {
    // generate an IP address for the player
    bool bAddedToNetwork = false;
    do
    {
      m_worldMgr.GenerateIPAddress(pPlayer->GetCountryID(), pPlayer->GetCityID(), pPlayer->GetIP());
      bAddedToNetwork = m_worldMgr.AddToNetwork( pPlayer->GetIP(), WorldManager::NetConnection::Player, pPlayer->GetID() );
    } while ( !bAddedToNetwork );

    // now that we have the player object, we need to load the player's facilities, items etc
    DBJobContext* pCtx = new DBJobContext;
    pCtx->pThis = this;
    pCtx->clientSocket = d.clientSocket;
    pCtx->pRouter = pRouter;
    pCtx->pData = (void*)pPlayer;

    GetDatabase().ExecuteJob(DBQ_LOAD_CHARACTER_COMPUTER, (void*)pCtx, d.character_id);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandCharacterAssetRequest(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCPKT_CHARACTER_ASSET_REQUEST d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByID( d.character_id )) )
  {
    SendCharacterAssetResponse( pPlayer, pRouter, clientSocket );
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandGetDesktopOptions(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCPKT_GET_DESKTOP_OPTIONS d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByID( d.character_id )) )
  {
    // return all available desktop options that the player has
    SendCharacterDesktopOptions( pPlayer, pRouter, clientSocket );
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandConsoleGetFSInfo(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_CON_GET_FS_INFO d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByID( d.character_id )) )
  {
    FileSystem& fs = pPlayer->GetComputer().GetFileSystem();

    SendConsoleFileSystemInfo( fs, pSocket, clientSocket );
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandConsoleGetFileList(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_CON_GET_FILE_LIST d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;
  vector<FileSystem::File> files;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket( clientSocket )) )
  {
    FileSystem& fs = pPlayer->GetComputer().GetFileSystem();

    fs.EnumerateFiles( d.currentDir, files );
    SendConsoleFileList(d.currentDir, files, pSocket, clientSocket);
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandConsoleCommand(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_CON_COMMAND d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket( clientSocket )) )
  {
    string cmd, args;
    size_t pos;

    // now we need to know how to execute commands
    FileSystem& fs = pPlayer->GetComputer().GetFileSystem();

    cmd = d.command;
    pos = cmd.find_first_of(" ");
    if ( pos != string::npos )
    {
      args = cmd.substr(cmd.find_first_of(" ")+1, cmd.length());
      cmd.erase(cmd.find_first_of(" "), cmd.length());
    }

    string result = fs.ExecuteCommand(cmd, args);
    SendConsoleCommandResult(pPlayer, result, pSocket, clientSocket);
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandForumGetThreads(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_FORUM_GET_THREADS d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByID(d.character_id)) )
  {
    // TODO: We need to load missions during startup.
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandClientDisconnect(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCSPKT_CLIENT_DISCONNECT d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  // try and retrieve the player object
  if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(d.clientSocket)) )
  {
    Player* pEventData = new Player( *pPlayer );
    EventSystem::GetInstance()->Emit( NULL, NULL, new EventWithDisposableData<Player>(Player::EVT_LoggedOut, pEventData) );

    m_playerMgr.RemovePlayer(pPlayer);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
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

bool FCLogicWorld::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadItemTypes(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicWorld* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();

  if ( pThis->HasConsole() )
    printf("%ld Item Types loaded\n", rowCount);

  for ( size_t i = 0; i < rowCount; i++ )
  {
    FCULONG id = resultSet.GetULongValue( "itemtype_id", i );
    string name = resultSet.GetStringValue( "itemtype_name", i );
    string dbTable = resultSet.GetStringValue( "itemtype_table", i );
    
    pThis->m_itemMgr.AddItemType(id, name, dbTable);
  }

  delete pCtx;
  pContext = NULL;

  // now try and load the items
  if ( pThis->HasConsole() )
    printf("Loading Item Definitions\n", rowCount);

  pCtx = new DBJobContext;
  pCtx->pThis = pThis;
  pThis->GetDatabase().ExecuteJob(DBQ_LOAD_ITEM_DEFS, (void*)pCtx);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadItemDefs(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicWorld* pThis = pCtx->pThis;
  ItemType* pType = NULL;
  stringstream queryStream;
  FCDatabase& db = pThis->GetDatabase();

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();
  string strObjDataQuery;

  if ( pThis->HasConsole() )
    printf("%ld Item Definitions loaded\n", rowCount);

  // create items with the core item data
  for ( size_t i = 0; i < rowCount; i++ )
  {
    FCULONG id = resultSet.GetULongValue("item_id", i);
    string name = resultSet.GetStringValue("item_name", i);
    FCULONG typeID = resultSet.GetULongValue("itemtype_id", i);
    FCULONG objID = resultSet.GetULongValue("object_id", i);
    FCSHORT minLevel = resultSet.GetShortValue("min_level", i);
    FCSHORT maxLevel = resultSet.GetShortValue("max_level", i);
    FCULONG npcValue = resultSet.GetULongValue("npc_value", i);

    // create the item
    if ( pThis->m_itemMgr.AddItem(id, name, typeID, objID, minLevel, maxLevel, npcValue) )
    {
      static char buffer[32000];

      memset(buffer, 0, sizeof(buffer));
      // if we successfully created the item, we now need to 'queue' the query to fetch its underlying object data
      switch ( typeID )
      {
      case  ItemType::Processor:
        sprintf(buffer, db.GetQueryByName(DBQ_LOAD_PROCESSOR_ITEM).c_str(), id, typeID, objID);
        break;

      case  ItemType::OS:
        sprintf(buffer, db.GetQueryByName(DBQ_LOAD_OS_ITEM).c_str(), id, typeID, objID);
        break;

      case  ItemType::Memory:
        sprintf(buffer, db.GetQueryByName(DBQ_LOAD_MEMORY_ITEM).c_str(), id, typeID, objID);

      default:
        break;
      }

      if ( strlen(buffer) )
      {
        queryStream << buffer << ';';
      }
    }
  }
  delete pCtx;
  pContext = NULL;

  // create the multiline query for the object data and queue it
  strObjDataQuery = queryStream.str();

  pCtx = new DBJobContext;
  pCtx->pThis = pThis;
  pCtx->pData = (void*)rowCount;
  db.ExecuteDirectQuery(strObjDataQuery, DBQ_LOAD_OBJECT_DATA, (void*)pCtx);
}

///////////////////////////////////////////////////////////////////////
// This callback will be called once per item, each resultset 
// containing one object.
///////////////////////////////////////////////////////////////////////
void FCLogicWorld::OnDBJob_LoadObjectData(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicWorld* pThis = pCtx->pThis;
  size_t objectsLeftToLoad = (size_t)(pCtx->pData);

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();

  FCULONG itemID = resultSet.GetULongValue("item_id", 0);
  FCULONG typeID = resultSet.GetULongValue("type_id", 0);

  switch ( typeID )
  {
  case  ItemType::Processor:
    {
      ItemProcessor* pItem = (ItemProcessor*)pThis->m_itemMgr.GetItem(itemID);

      if ( pItem )
      {
        pItem->SetCoreCount( resultSet.GetShortValue("core_count", 0) );
        pItem->SetCoreSpeed( resultSet.GetShortValue("core_speed", 0) );
      }
    }
    break;

  case  ItemType::OS:
    {
      ItemOS* pItem = (ItemOS*)pThis->m_itemMgr.GetItem(itemID);

      if ( pItem )
      {
        pItem->SetKernelID( resultSet.GetShortValue("oskernel_id", 0) );
        pItem->SetKernelName( resultSet.GetStringValue("name", 0) );
      }
    }
    break;

  case  ItemType::Memory:
    {
      ItemMemory* pItem = (ItemMemory*)pThis->m_itemMgr.GetItem(itemID);

      if ( pItem )
      {
        pItem->SetMemorySize( resultSet.GetULongValue("memory_size", 0) );
      }
    }
  }

  if ( resultSet.IsLastResultSet() )
  {
    if ( pThis->HasConsole() )
      printf("%ld objects were loaded!\n", objectsLeftToLoad);

    delete pCtx;
    pContext = NULL;

    // at this point, we should be signalling to the rest of the service to wake up and continue starting up.
    pthread_cond_signal(&pThis->m_condSync);
  }
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadCharacterComputer(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*)pContext;
  RouterSocket* pSock = pCtx->pRouter;
  FCLogicWorld* pThis = pCtx->pThis;
  Player* pPlayer = (Player*)pCtx->pData;

  if ( !pThis || !pPlayer )
    return;

  // set the main computer details
  Computer& comp = pPlayer->GetComputer();
  pThis->UpdateComputerFromResultSet(comp, resultSet);

  // we are done loading the player... notify the client that the character has been created...
  pThis->SendCharacterLoginStatus(pPlayer->GetAccountID(), pPlayer->GetID(), CharacterSelectSucceeded, pSock, pCtx->clientSocket);

  // emit an event for the player logging in
  EventSystem::GetInstance()->Emit( pPlayer, NULL, new Event(Player::EVT_LoggedIn, (void*)pPlayer) );

  delete pCtx;
  pContext = NULL;
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadWorldGeography(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  FCLogicWorld* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  // load the countries and cities
  size_t rowCount = resultSet.GetRowCount();
  FCULONG countryID, cityID;
  FCSHORT countryIP, cityIP;
  string countryName, cityName;
  Country* pCountry = NULL;

  for ( size_t i = 0; i < rowCount; i++ )
  {
    countryID = resultSet.GetULongValue("country_id", i);
    countryName = resultSet.GetStringValue("country_name", i);
    countryIP = resultSet.GetShortValue("IP_groupA", i);
    cityID = resultSet.GetULongValue("city_id", i);
    cityName = resultSet.GetStringValue("city_name", i);
    cityIP = resultSet.GetShortValue("IP_groupB", i);

    // create the country...
    if ( (pCountry = pThis->m_worldMgr.AddCountry(countryID, countryName, countryIP)) )
    {
      pThis->m_worldMgr.AddCity(pCountry, cityID, cityName, cityIP);
    }
  }

  delete pCtx;
  pContext = NULL;

  if ( pThis->HasConsole() )
  {
    printf("%ld locations loaded\n", rowCount);
  }

  pCtx = new DBJobContext;
  pCtx->pThis = pThis;
  pThis->GetDatabase().ExecuteJob(DBQ_LOAD_COMPANIES, (void*)pCtx);

}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadCompanies(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  FCLogicWorld* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();
  FCULONG companyID, cityID;
  string name;
  InGameIPAddress ip;

  for ( size_t i = 0; i < rowCount; i++ )
  {
    companyID = resultSet.GetULongValue("company_id", i);
    name = resultSet.GetStringValue("company_name", i);
    cityID = resultSet.GetULongValue("city_id", i);
    ip.SetIP(resultSet.GetShortValue("IP_groupA", i), 
             resultSet.GetShortValue("IP_groupB", i),
             resultSet.GetShortValue("IP_groupC", i),
             resultSet.GetShortValue("IP_groupD", i));

    pThis->m_worldMgr.AddCompany(companyID, name, cityID, ip);
  }

  delete pCtx;
  pContext = NULL;

  if ( pThis->HasConsole() )
    printf("%ld companies loaded\n", rowCount);

  pCtx = new DBJobContext;
  pCtx->pThis = pThis;
  pThis->GetDatabase().ExecuteJob(DBQ_LOAD_COMPANY_COMPUTERS, (void*)pCtx);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadCompanyComputers(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  FCLogicWorld* pThis = pCtx->pThis;
  Company* pCompany = NULL;

  if ( !pThis )
    return;

  FCULONG companyID;

  size_t rowCount = resultSet.GetRowCount();

  for ( size_t i = 0; i < rowCount; i++ )
  {
    companyID = resultSet.GetULongValue("owner_id", i);
    if ( (pCompany = pThis->m_worldMgr.GetCompany(companyID)) )
    {
      Computer& comp = pCompany->GetComputer();

      pThis->UpdateComputerFromResultSet(comp, resultSet, i);
      pThis->m_worldMgr.AddToNetwork( pCompany->GetIP(), WorldManager::NetConnection::Company, companyID );
    }
    else
    {
      // failed to find company that this computer belongs to...
    }
  }

  delete pCtx;
  pContext = NULL;

  if ( pThis->HasConsole() )
    printf("%ld company computers loaded\n", rowCount);

  pthread_cond_signal(&pThis->m_condSync);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadMissions(DBIResultSet& resultSet, void*& pContext)
{
  DBJobContext* pCtx = (DBJobContext*) pContext;
  FCLogicWorld* pThis = pCtx->pThis;

  if ( !pThis )
    return;

  size_t rowCount = resultSet.GetRowCount();
  FCULONG id, parentID;
  FCSHORT minLevel, maxLevel, difficulty, successCount, failureCount;
  string eventSuccess, eventFailure;

  for ( size_t i = 0; i < rowCount; i++ )
  {
    id = resultSet.GetULongValue("mission_id", i);
    parentID = resultSet.GetULongValue("parentmission_id", i);
    minLevel = resultSet.GetShortValue("min_level", i);
    maxLevel = resultSet.GetShortValue("max_level", i);
    difficulty = resultSet.GetShortValue("difficulty", i);
    eventSuccess = resultSet.GetStringValue("success_event_id", i);
    eventFailure = resultSet.GetStringValue("failure_event_id", i);
    successCount = resultSet.GetShortValue("success_count", i);
    failureCount = resultSet.GetShortValue("failure_count", i);

    pThis->m_missionMgr.AddMission( id, minLevel, maxLevel, difficulty, eventSuccess, eventFailure, parentID, successCount, failureCount );
  }

  delete pCtx;
  pContext = NULL;

  if ( pThis->HasConsole() )
    printf("%ld missions loaded\n", rowCount);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::UpdateComputerFromResultSet(Computer& comp, DBIResultSet& resultSet, size_t row)
{
  FCULONG ownerID = 0; 
  FCSHORT ownertypeID = 0; 

  ownerID = resultSet.GetULongValue("owner_id", row);
  ownertypeID = resultSet.GetShortValue("ownertype_id", row);

  comp.SetID( resultSet.GetULongValue("computer_id", row) );
  comp.SetName( resultSet.GetStringValue("name", row) );
  comp.SetNetworkSpeed( resultSet.GetULongValue("network_speed", row) );
  comp.SetHDDSize( resultSet.GetULongValue("harddrive_size", row) );

  // set the sub-component details (cpu, ram, network, os...)
  ItemProcessor& cpu = comp.GetProcessor();
  ItemOS& os = comp.GetOS();
  ItemMemory& mem = comp.GetMemory();
  ItemProcessor* pProcessor = (ItemProcessor*)m_itemMgr.GetItem( resultSet.GetULongValue("processor_id", row) );
  ItemOS* pOS = (ItemOS*)m_itemMgr.GetItem( resultSet.GetULongValue("os_id", row) );
  ItemMemory* pMem = (ItemMemory*)m_itemMgr.GetItem( resultSet.GetULongValue("memory_id", row) );

  if ( pProcessor )  
    cpu = *pProcessor;
  if ( pOS )         
    os = *pOS;
  if ( pMem )        
    mem = *pMem;

  // load the computer's file system
  stringstream ss;

  ss << m_pathFileSystems << "fs_uid_" << ownerID << ".xml";
  comp.GetFileSystem().LoadFromXML( ss.str() );
}