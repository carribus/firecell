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
#include "Forum.h"
#include "ForumCategory.h"
#include "PacketDispatcher.h"
#include "world_comms.h"
#include "FCLogicWorld.h"

#define DBQ_LOAD_OBJECT_DATA    "load_object_data"

FCLogicWorld::FCLogicWorld()
: ServiceLogicBase("FireCell World Service", false)
{
}

///////////////////////////////////////////////////////////////////////

FCLogicWorld::~FCLogicWorld(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::Free()
{
  Stop();
  delete this;
}

///////////////////////////////////////////////////////////////////////

int FCLogicWorld::Start()
{
  string strEngine, strServer, strDBName, strUser, strPass;

  stringstream ss;

  // load the configuration
  if ( !LoadConfig("FCWorld.conf") )
  {
    DYNLOG_ADDLOG("Failed to load FCWorld.conf configuration file");
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

  // initialise the packet dispatcher
  PacketDispatcher::instance().initialise(1);

  /*
   *  Initialise the Database object
   */
  DYNLOG_ADDLOG("Setting up database connection...");

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
		RegisterDBHandler(DBQ_LOAD_FORUM_CATEGORIES, OnDBJob_LoadForumCategories);
		RegisterDBHandler(DBQ_LOAD_FORUM_POSTS, OnDBJob_LoadForumPosts);

    // Start the Item loading...
    DYNLOG_ADDLOG("Loading Item Types...");

    DBJobContext* pCtx = new DBJobContext;
    pCtx->pThis = this;
    GetDatabase().ExecuteJob(DBQ_LOAD_ITEM_TYPES, (void*)pCtx);

    // we want to pause here until the item data required data is loaded
    m_condSync.WaitForSignal();

    LoadWorldData();
  }

  /*
   *  Configure the Event System and set up dependencies
   */
  ConfigureEventSystem();
  m_playerMgr.SetEventSystem( EventSystem::GetInstance() );
	m_missionMgr.SetEventSystem( EventSystem::GetInstance() );

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
	// load the world related data
  DBJobContext* pCtx = new DBJobContext;
  pCtx->pThis = this;
  GetDatabase().ExecuteJob(DBQ_LOAD_WORLD_GEOGRAPHY, (void*)pCtx);

  // we want to pause here until the item data required data is loaded
  m_condSync.WaitForSignal();

  // load missions
  pCtx = new DBJobContext;
  pCtx->pThis = this;
  GetDatabase().ExecuteJob(DBQ_LOAD_MISSIONS, (void*)pCtx);

  // we want to pause here until the mission data required data is loaded
  m_condSync.WaitForSignal();

	// load forum data
	pCtx = new DBJobContext;
	pCtx->pThis = this;
	GetDatabase().ExecuteJob(DBQ_LOAD_FORUM_CATEGORIES, (void*)pCtx);

  // we want to pause here until the forum data required data is loaded
  m_condSync.WaitForSignal();
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

  case  FCMSG_GET_CHAR_CREATION_PARAMS:
    {
      bHandled = OnCommandGetCharacterCreationParams(pPkt, pRouter, clientSock);
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

	case	FCMSG_ACTIVATE_DESKTOP_OPTION:
		{
			bHandled = OnCommandActivateDesktopOption(pPkt, pRouter, clientSock);
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
	case	FCMSG_FORUM_GET_CATEGORIES:
		{
			bHandled = OnCommandForumGetCategories(pPkt, pRouter, clientSock);
		}
		break;

	case	FCMSG_FORUM_GET_THREADS:
    {
      bHandled = OnCommandForumGetThreads(pPkt, pRouter, clientSock);
    }
		break;

	case	FCMSG_FORUM_GET_THREAD_DETAILS:
		{
			bHandled = OnCommandForumGetThreadDetails(pPkt, pRouter, clientSock);
		}
		break;

  case  FCMSG_FORUM_GET_THREAD_CONTENTBLOB:
    {
      bHandled = OnCommandForumGetThreadContentBlob(pPkt, pRouter, clientSock);
    }
    break;

  case  FCMSG_FORUM_CREATE_NEW_THREAD:
    {
      bHandled = OnCommandForumCreateNewThread(pPkt, pRouter, clientSock);
    }
    break;

	/*
	 *	Mission Messages
	 */
	case	FCMSG_MISSION_ACCEPT:
		{
			bHandled = OnCommandMissionAccept(pPkt, pRouter, clientSock);
		}
		break;

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
    // set the router socket to the player
    pPlayer->SetRouterSocket(pRouter);
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

bool FCLogicWorld::OnCommandGetCharacterCreationParams(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
  __FCPKT_GET_CHAR_CREATION_PARAMS d;
  size_t dataLen = 0;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

  std::vector<Country> arrCountries;
  std::vector<City> arrCities;
  FCUINT countriesAvailable = 0, citiesAvailable = 0;         // count of countries and cities that support character creation

  // populate the packet with the available countries
  m_worldMgr.GetAllCountries(arrCountries, true);

  // populate the packet with the available cities
  m_worldMgr.GetAllCities(arrCities, true);

  SendCharCreationParameters(arrCountries, arrCities, pRouter, clientSocket);

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

bool FCLogicWorld::OnCommandActivateDesktopOption(PEPacket* pPkt, RouterSocket* pRouter, FCSOCKET clientSocket)
{
	__FCPKT_ACTIVATE_DESKTOP_OPTION d;
	size_t dataLen = 0;
	Player* pPlayer = NULL;
	
  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

	if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
	{
		// calculate whether the player can run the option
    if ( 1 )
    {
		  SendActivateDesktopOptionResponse( d.optionID, pPlayer, pRouter, clientSocket );
    }
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

    string result = fs.ExecuteCommand(pPlayer, cmd, args);
    SendConsoleCommandResult(pPlayer, result, pSocket, clientSocket);
  }
  else
    return false;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandForumGetCategories(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
	__FCPKT_FORUM_GET_CATEGORIES d;
  size_t dataLen = 0;
  Player* pPlayer = NULL;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", (void*)&d, dataLen);

	if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
	{
		// TODO: Create a forum object that can handle these requests, and return required results for this and other forum messages
		vector<ForumCategory*> arrCategories;

		m_forum.GetCategoriesForPlayer(pPlayer, arrCategories);
		SendForumCategories(arrCategories, pSocket, clientSocket);
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

  if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
  {
		vector<ForumPost*> target;
		m_forum.GetForumThreads( d.category_id, target );

		SendForumThreads(d.category_id, target, pSocket, clientSocket);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandForumGetThreadDetails(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
	__FCPKT_FORUM_GET_THREAD_DETAILS d;
	size_t dataLen = 0;
	Player* pPlayer = NULL;

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	pPkt->GetField("data", (void*)&d, dataLen);

	if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
	{
    ForumCategory* pCat = m_forum.GetCategoryByID(d.category_id);

    if ( pCat )
    {
      ForumPost* pPost = pCat->GetPostByID(d.thread_id);

      if ( pPost )
      {
        SendForumThreadDetails(d.category_id, pPost, pSocket, clientSocket);
      }
    }
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandForumGetThreadContentBlob(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_FORUM_GET_THREAD_CONTENTBLOB d;
	size_t dataLen = 0;
	Player* pPlayer = NULL;

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	pPkt->GetField("data", (void*)&d, dataLen);

	if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
	{
    ForumCategory* pCat = m_forum.GetCategoryByID(d.category_id);

    if ( pCat )
    {
      ForumPost* pPost = pCat->GetPostByID(d.thread_id);

      if ( pPost )
      {
        SendForumThreadContentBlob(d.category_id, pPost, pSocket, clientSocket);
      }
    }
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandForumCreateNewThread(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
  __FCPKT_FORUM_CREATE_NEW_THREAD* d = NULL;
  size_t dataLen = 0;
  Player* pPlayer = NULL;
	bool bSuccess = true;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_FORUM_CREATE_NEW_THREAD*) new FCBYTE[dataLen];
  pPkt->GetField("data", (void*)d, dataLen);

  if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
  {
    std::string message;
    message.assign( d->content, d->contentLength );
    if ( !(bSuccess = m_forum.CreateNewForumThread( d->category_id, d->thread_id, pPlayer->GetID(), pPlayer->GetName(), d->title, message )) )
    {
      DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to add new forum thread to categoryID:%ld (Author:%s[%ld])", d->category_id, pPlayer->GetName().c_str(), pPlayer->GetID()) );
    }
		SendForumCreateNewThreadResult(d->category_id, d->thread_id, bSuccess, pSocket, clientSocket);
  }

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCLogicWorld::OnCommandMissionAccept(PEPacket* pPkt, RouterSocket* pSocket, FCSOCKET clientSocket)
{
	__FCPKT_MISSION_ACCEPT d;
	size_t dataLen = 0;
	Player* pPlayer = NULL;
	Mission* pMission = NULL;

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	pPkt->GetField("data", (void*)&d, dataLen);

	if ( (pPlayer = m_playerMgr.GetPlayerByClientSocket(clientSocket)) )
	{
		// find the mission that the player accepted
		if ( (pMission = m_missionMgr.GetMission( d.mission_id )) )
		{
			if ( pMission->CanPlayerAccept(pPlayer) )
			{
				if ( m_missionMgr.AssignMissionToPlayer(pPlayer, d.mission_id) )
        {
          // successfully assigned the mission (and any child missions) to the player...
          // send back a response indicating which missions got added to the player's mission list
          SendMissionAcceptedResponse(pPlayer, d.mission_id, true, pSocket, clientSocket);
        }
        else
				{
					DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to assign mission id %ld to player id %ld", d.mission_id, pPlayer->GetID()) );
				}
			}
		}
		else
    {
			DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to find mission id %ld in the mission manager", d.mission_id) );
    }
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
          DYNLOG_ADDLOG( DYNLOG_FORMAT("Service registered with Router (%s:%ld)", pRouter->GetServer().c_str(), pRouter->GetPort()) );
        }
        else
        {
          // registration failed
          DYNLOG_ADDLOG( DYNLOG_FORMAT("Service failed to register with Router (%s:%ld)", pRouter->GetServer().c_str(), pRouter->GetPort()) );
        }
      }
    }
    break;

  default:
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Unknown Message Received (%ld)", msgID) );
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

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld Item Types loaded", rowCount));

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
  DYNLOG_ADDLOG( DYNLOG_FORMAT("Loading Item Definitions", rowCount) );

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

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld Item Definitions loaded", rowCount) );

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
    DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld objects were loaded!", objectsLeftToLoad) );

    delete pCtx;
    pContext = NULL;

    // at this point, we should be signalling to the rest of the service to wake up and continue starting up.
    pThis->m_condSync.Signal();
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
  SendCharacterLoginStatus(pPlayer->GetAccountID(), pPlayer->GetID(), CharacterSelectSucceeded, pSock, pCtx->clientSocket);

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
  FCBYTE supportsCharCreation;
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
    supportsCharCreation = resultSet.GetByteValue("support_char_creation", i);

    // create the country...
    if ( (pCountry = pThis->m_worldMgr.AddCountry(countryID, countryName, countryIP, (supportsCharCreation ? true : false))) )
    {
      pThis->m_worldMgr.AddCity(pCountry, cityID, cityName, cityIP, (supportsCharCreation ? true : false));
    }
  }

  delete pCtx;
  pContext = NULL;

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld locations loaded", rowCount) );

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

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld companies loaded", rowCount) );

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

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld company computers loaded", rowCount) );

  pThis->m_condSync.Signal();
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
  FCSHORT minLevel, maxLevel, difficulty, successCount, failureCount, xpSuccess, xpFailure;
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
    xpSuccess = resultSet.GetShortValue("xp_success", i);
    xpFailure = resultSet.GetShortValue("xp_failure", i);

    pThis->m_missionMgr.AddMission( id, minLevel, maxLevel, difficulty, eventSuccess, eventFailure, xpSuccess, xpFailure, parentID, successCount, failureCount );
  }

  delete pCtx;
  pContext = NULL;

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld missions loaded", rowCount) );

  pThis->m_condSync.Signal();
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadForumCategories(DBIResultSet& resultSet, void*& pContext)
{
	DBJobContext* pCtx = (DBJobContext*) pContext;
	FCLogicWorld* pThis = pCtx->pThis;

	if ( !pThis )
		return;

	size_t rowCount = resultSet.GetRowCount();
	FCULONG id, parentID, accountTypeReq, minLevel, maxLevel, order;
	string name, desc;

	for ( size_t i = 0; i < rowCount; i++ )
	{
		id = resultSet.GetULongValue("forumcat_id", i);
		parentID = resultSet.GetULongValue("parent_id", i);
		order = resultSet.GetULongValue("order", i);
		name = resultSet.GetStringValue("name", i);
		desc = resultSet.GetStringValue("description", i);
		accountTypeReq = resultSet.GetULongValue("accounttype_required", i);
		minLevel = resultSet.GetULongValue("min_level", i);
		maxLevel = resultSet.GetULongValue("max_level", i);

		pThis->m_forum.AddCategory(id, parentID, order, name, desc, accountTypeReq, minLevel, maxLevel);
	}

	delete pCtx;
	pContext = NULL;

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld forum categories loaded", rowCount) );

	pCtx = new DBJobContext;
  pCtx->pThis = pThis;
  pThis->GetDatabase().ExecuteJob(DBQ_LOAD_FORUM_POSTS, (void*)pCtx);
}

///////////////////////////////////////////////////////////////////////

void FCLogicWorld::OnDBJob_LoadForumPosts(DBIResultSet& resultSet, void*& pContext)
{
	DBJobContext* pCtx = (DBJobContext*) pContext;
	FCLogicWorld* pThis = pCtx->pThis;

	if ( !pThis )
		return;

	size_t rowCount = resultSet.GetRowCount();
	FCULONG post_id, parent_id, category_id, order, author_id, mission_id;
	bool locked;
	string title, author, content, date_created;

	for ( size_t i = 0; i < rowCount; i++ )
	{
		post_id = resultSet.GetULongValue("forumpost_id", i);
		parent_id = resultSet.GetULongValue("parentpost_id", i);
		category_id = resultSet.GetULongValue("forumcategory_id", i);
		order = resultSet.GetULongValue("order", i);
		title = resultSet.GetStringValue("title", i);
		author_id = resultSet.GetULongValue("author_id", i);
    author = resultSet.GetStringValue("author_name", i);
		content = resultSet.GetStringValue("content", i);
		date_created = resultSet.GetStringValue("date_created", i);
		mission_id = resultSet.GetULongValue("mission_id", i);
    locked = resultSet.GetByteValue("locked", i) ? true : false;

		pThis->m_forum.AddForumPost(post_id, parent_id, category_id, order, title, content, author_id, author, date_created, mission_id);
	}

  DYNLOG_ADDLOG( DYNLOG_FORMAT("%ld forum posts loaded", rowCount) );

	delete pCtx;
	pContext = NULL;

  pThis->m_condSync.Signal();
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

  switch ( ownertypeID )
  {
  case  1:      // Player
    ss << m_pathFileSystems << "fs_uid_" << ownerID << ".xml";
    break;

  case  2:      // NPC
    ss << m_pathFileSystems << "fs_npcid_" << ownerID << ".xml";
    break;

  case  3:      // Company
    ss << m_pathFileSystems << "fs_coid_" << ownerID << ".xml";
    break;

  default:      // Unknown owner type
    return;
  }
  comp.GetFileSystem().LoadFromXML( ss.str() );
}