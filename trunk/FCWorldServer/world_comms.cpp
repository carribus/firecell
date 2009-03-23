#include "../common/PEPacket.h"
#include "../common/PEPacketHelper.h"
#include "PacketDispatcher.h"
#include "world_comms.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharacterLoginStatus(FCULONG accountID, FCULONG characterID, e_SelectCharacterStatus status, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_SELECT_CHARACTER_RESP d;

  // check if the action failed...
  if ( status != CharacterSelectSucceeded )
  {
    ServiceType target = ST_Auth;
    __FCSPKT_CHARACTER_LOGGEDIN_ERROR dError;

    dError.account_id = accountID;
    dError.character_id = characterID;
    dError.status = status;

    PEPacketHelper::CreatePacket(*pkt, FCPKT_ERROR, FCSMSG_CHARACTER_LOGGEDIN, ST_Auth);
    PEPacketHelper::SetPacketData(*pkt, (void*)&dError, sizeof(dError));

    QueuePacket(pkt, pRouter);

    pkt = new PEPacket;
  }

  d.character_id = characterID;
  d.status = status;

  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_SELECT_CHARACTER, ST_None);
  PEPacketHelper::SetPacketData(*pkt, (void*)&d, sizeof(__FCPKT_SELECT_CHARACTER_RESP));

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharCreationParameters(std::vector<Country>& countries, std::vector<City>& cities, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  size_t numCountries = countries.size();
  size_t numCities = cities.size();

  if ( !numCountries )
  {
    DYNLOG_ADDLOG("Unable to send character creation parameters because number of COUNTRIES available for character creation is ZERO");
    return;
  }

  if ( !numCities )
  {
    DYNLOG_ADDLOG("Unable to send character creation parameters because number of CITIES available for character creation is ZERO");
    return;
  }

  SendCharCreationParameters_Countries(countries, pRouter, clientSocket);
  SendCharCreationParameters_Cities(cities, pRouter, clientSocket);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharCreationParameters_Countries(std::vector<Country>& countries, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP* d = NULL;
  FCUINT index = 0;
  size_t nPktLen = 0;
  size_t numCountries = countries.size();

  // calculate the packet length
  nPktLen = sizeof(__FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP) + 
            ((numCountries-1) * sizeof(__FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP::Country));

  d = (__FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP*) new FCBYTE[nPktLen];
  if ( d )
  {
    d->respType = 0;
    d->numCountries = (FCULONG)numCountries;
    // copy the countries into the packet
    std::vector<Country>::iterator countryIt = countries.begin();
    std::vector<Country>::iterator countryLimit = countries.end();

    for ( ; countryIt != countryLimit; countryIt++, index++ )
    {
      d->countries[index].country_id = countryIt->GetID();
      memset( d->countries[index].name, 0, sizeof(d->countries[index].name) );
      strncpy( d->countries[index].name, countryIt->GetName().c_str(), countryIt->GetName().size() );
    }

    // send the packet
    PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_GET_CHAR_CREATION_PARAMS, ST_None);
    PEPacketHelper::SetPacketData(*pkt, 
                                  (void*)d, 
                                  nPktLen);

    // send notification to Client
    pkt->SetFieldValue("target", (void*)&clientSocket);
    QueuePacket(pkt, pRouter);

    // clear the data portion
    delete [] d;
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to allocate memory (%ld bytes) for packet __FCPKT_GET_CHAR_CREATION_PARAMS_COUNTRIES_RESP", nPktLen) );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharCreationParameters_Cities(std::vector<City>& cities, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP* d = NULL;
  FCUINT index = 0;
  size_t nPktLen = 0;
  size_t numCities = cities.size();

  // calculate the packet length
  nPktLen = sizeof(__FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP) +
    ((numCities-1) * sizeof(__FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP::City));

  d = (__FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP*) new FCBYTE[nPktLen];
  if ( d )
  {
    d->respType = 1;
    d->numCities = (FCULONG)numCities;
    // copy the cities into the packet
    std::vector<City>::iterator cityIt = cities.begin();
    std::vector<City>::iterator cityLimit = cities.end();

    for ( ; cityIt != cityLimit; cityIt++, index++ )
    {
      d->cities[index].country_id = cityIt->GetCountry()->GetID();
      d->cities[index].city_id = cityIt->GetID(); 
      memset( d->cities[index].name, 0, sizeof(d->cities[index].name) );
      strncpy( d->cities[index].name, cityIt->GetName().c_str(), cityIt->GetName().size() );
    }

    // send the packet
    PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_GET_CHAR_CREATION_PARAMS, ST_None);
    PEPacketHelper::SetPacketData(*pkt, 
                                  (void*)d, 
                                  nPktLen);

    // send notification to Client
    pkt->SetFieldValue("target", (void*)&clientSocket);
    QueuePacket(pkt, pRouter);

    // clear the data portion
    delete [] d;
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Failed to allocate memory (%ld bytes) for packet __FCPKT_GET_CHAR_CREATION_PARAMS_CITIES_RESP", nPktLen) );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharacterAssetResponse(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPlayer )
    return;

  PEPacket* pkt = new PEPacket;
  __FCPKT_CHARACTER_ASSET_REQUEST_RESP d;
  Computer& comp = pPlayer->GetComputer();

  // ip address
  pPlayer->GetIP().GetIP( d.ip_address.a, d.ip_address.b, d.ip_address.c, d.ip_address.d );

  // computer data
  d.computer.id = comp.GetID();
  strncpy( d.computer.name, comp.GetName().c_str(), sizeof(d.computer.name)-1 );
  d.computer.hddSize = comp.GetTotalHDD();
  d.computer.networkSpeed = comp.GetNetworkSpeed();
	d.computer.availablePorts = comp.GetNetworkPorts().getPortCount();

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

  // network ports
  NetworkPorts& ports = comp.GetNetworkPorts();

  for ( FCSHORT i = 0; i < d.computer.availablePorts; i++ )
  {
    d.computer.network_ports[i].portNum = i;
    d.computer.network_ports[i].enabled = ports.isPortEnabled(i);
    d.computer.network_ports[i].portMaxHealth = ports.getPortMaxHealth(i);
    d.computer.network_ports[i].portHealth = ports.getPortHealth(i);
    ports.getSoftwareInfo(i, d.computer.network_ports[i].itemID, d.computer.network_ports[i].softwareType );
  }

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CHARACTER_ASSET_REQUEST, ST_None);
  PEPacketHelper::SetPacketData(*pkt, (void*)&d, sizeof(__FCPKT_CHARACTER_ASSET_REQUEST_RESP));

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharacterItemsResponse(Player* pPlayer, ItemManager& itemMgr,  BaseSocket* pRouter, FCSOCKET clientSocket)
{
	if ( !pPlayer )
		return;

	PEPacket* pkt = new PEPacket;
	__FCPKT_CHARACTER_ITEMS_REQUEST_RESP* d = NULL;
	int nPktLen = 0;

  pPlayer->LockItemsForRead();

  const std::map<FCULONG, Player::PlayerItem>& items = pPlayer->GetItems();         // get a const reference to the player's items
  std::map<FCULONG, Player::PlayerItem>::const_iterator it = items.begin();
  std::map<FCULONG, Player::PlayerItem>::const_iterator limit = items.end();
  size_t itemCount = items.size(), index = 0;
  ItemSoftware* pItem = NULL;

  // calc the size of the packet
  nPktLen = sizeof(__FCPKT_CHARACTER_ITEMS_REQUEST_RESP) + ( ((int)itemCount-1) * sizeof(__FCPKT_CHARACTER_ITEMS_REQUEST_RESP::_software) );
  d = (__FCPKT_CHARACTER_ITEMS_REQUEST_RESP*) new FCBYTE[nPktLen];
  memset(d, 0, nPktLen);
  d->itemCount = (FCULONG)itemCount;

  // loop through the items and populate the packet
  for ( ; it != limit; it++ )
  {
    // get the item
    if ( (pItem = (ItemSoftware*)itemMgr.GetItem( it->first )) )
    {
      d->software[index].item_id = pItem->GetID();
      strncpy( d->software[index].name, pItem->GetName().c_str(), sizeof(d->software[index].name)-1 );
      d->software[index].itemtype_id = pItem->GetTypeID();
      d->software[index].min_level = pItem->GetMinLevel();
      d->software[index].max_level = pItem->GetMaxLevel();
      d->software[index].npc_value = pItem->GetNPCValue();

      d->software[index].softwareTypeID = pItem->GetSoftwareType();
      d->software[index].is_service = pItem->IsService();
      d->software[index].scriptID = 0;
      d->software[index].itemCount = it->second.count;
    }
    else
    {
      DYNLOG_ADDLOG( DYNLOG_FORMAT("SendCharacterItemsResponse(): Failed to find item ID %ld", it->first) );
    }
    index++;
  }

  pPlayer->UnlockItems();

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CHARACTER_ITEMS_REQUEST, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                nPktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendCharacterDesktopOptions(Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPlayer )
    return;

  PEPacket* pkt = new PEPacket;
  __FCPKT_GET_DESKTOP_OPTIONS_RESP* d = NULL;
  int nPktLen = 0;
  int numOptions = 7;
  char* OptionNames[] =
  {
    "Hacking Forum",
    "Web Browser",
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
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_GET_DESKTOP_OPTIONS, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                nPktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendActivateDesktopOptionResponse(FCULONG optionID, Player* pPlayer, BaseSocket* pRouter, FCSOCKET clientSocket)
{
	PEPacket* pkt = new PEPacket;
	Computer& comp = pPlayer->GetComputer();
	__FCPKT_ACTIVATE_DESKTOP_OPTION_RESP d;
	
	// TODO: Need to add the necessary methods to calculate whether an application can run
	d.optionID = optionID;
	d.canActivate = true;
	d.cpu_cost = 0;
	d.mem_cost = 0;

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_ACTIVATE_DESKTOP_OPTION, ST_None);
  PEPacketHelper::SetPacketData(*pkt, (void*)&d, sizeof(__FCPKT_ACTIVATE_DESKTOP_OPTION_RESP));

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Console send methods */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendConsoleFileSystemInfo(FileSystem& fs, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_CON_GET_FS_INFO_RESP d;

  d.fsStyle = fs.GetFSStyle();
  strncpy(d.dirSeperator, fs.GetDirSeperator().c_str(), sizeof(d.dirSeperator));
  strncpy(d.currentDir, fs.GetCurrentPathName().c_str(), sizeof(d.currentDir));

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CON_GET_FS_INFO, ST_None);
  PEPacketHelper::SetPacketData(*pkt, (void*)&d, sizeof(__FCPKT_CON_GET_FS_INFO_RESP));

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendConsoleFileList(string currentDir, vector<FileSystem::File> files, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
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
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CON_GET_FILE_LIST, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                nPktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendConsoleCommandResult(Player* pPlayer, string result, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
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
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_CON_COMMAND, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                pktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Forum send methods */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendForumCategories(vector<ForumCategory*>& categories, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_FORUM_GET_CATEGORIES_RESP* d = NULL;
	size_t catCount = categories.size(), index = 0;
	size_t pktLen = sizeof(__FCPKT_FORUM_GET_CATEGORIES_RESP) + ( (catCount-1) * sizeof(__FCPKT_FORUM_GET_CATEGORIES_RESP::category_info));

	d = (__FCPKT_FORUM_GET_CATEGORIES_RESP*) new FCBYTE[ pktLen ];
	memset(d, 0, pktLen);
	d->category_count = (FCSHORT)catCount;

	ForumCategory* pCat = NULL;
	vector<ForumCategory*>::iterator it = categories.begin();
	while ( it != categories.end() )
	{
		pCat = (*it);
		d->categories[index].category_id = pCat->GetID();
		d->categories[index].parent_id = pCat->GetParentID();
    d->categories[index].thread_count = (FCULONG)pCat->GetPostCount();
		strncpy( d->categories[index].name, pCat->GetName().c_str(), sizeof(d->categories[index].name) );
		strncpy( d->categories[index].desc, pCat->GetDescription().c_str(), sizeof(d->categories[index].desc) );
		it++;
		index++;
	}

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_FORUM_GET_CATEGORIES, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                pktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendForumThreads(FCULONG category_id, vector<ForumPost*>& threads, BaseSocket* pRouter, FCSOCKET clientSocket)
{
	PEPacket* pkt = new PEPacket;
	__FCPKT_FORUM_GET_THREADS_RESP* d;
	size_t threadCount = threads.size(), index = 0;
	size_t pktLen = sizeof(__FCPKT_FORUM_GET_THREADS_RESP) + ( (threadCount-1) * sizeof(__FCPKT_FORUM_GET_THREADS_RESP::thread_data));

	d = (__FCPKT_FORUM_GET_THREADS_RESP*) new FCBYTE[ pktLen ];
	memset(d, 0, pktLen);
	d->thread_count = (FCSHORT)threadCount;
	d->category_id = category_id;

	vector<ForumPost*>::iterator it = threads.begin();

	while ( it != threads.end() )
	{
		d->threads[index].thread_id = (*it)->GetID();
		d->threads[index].parent_id = (*it)->GetParentID();
		d->threads[index].author_id = (*it)->GetAuthorID();
		strncpy( d->threads[index].author_name, (*it)->GetAuthorName().c_str(), sizeof(d->threads[index].author_name) );
		d->threads[index].mission_id = (*it)->GetMissionID();
		d->threads[index].order = (*it)->GetOrder();
		strncpy( d->threads[index].title, (*it)->GetTitle().c_str(), sizeof(d->threads[index].title) );
		strncpy( d->threads[index].date_created, (*it)->GetDateCreated().c_str(), sizeof(d->threads[index].date_created) );
		it++;
		index++;
	}

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_FORUM_GET_THREADS, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)d, 
                                pktLen);

  // send notification to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);

  // clear the data portion
  delete [] (FCBYTE*)d;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendForumThreadDetails(FCULONG category_id, ForumPost* pPost, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPost )
  {
    DYNLOG_ADDLOG("FCLogicWorld::SendForumThreadDetails() failed due to pPost == NULL");
    return;
  }

  PEPacket* pkt = new PEPacket;
  __FCPKT_FORUM_GET_THREAD_DETAILS_RESP* d = NULL;
  size_t pktLen = sizeof(__FCPKT_FORUM_GET_THREAD_DETAILS_RESP);
  const vector<ForumPost*>& posts = pPost->GetPosts();
  vector<ForumPost*>::const_iterator it = posts.begin();
  vector<ForumPost*>::const_iterator limit = posts.end();
  FCULONG numPosts = (FCULONG)posts.size(), contentLen = (FCULONG)pPost->GetContent().size(), postIndex = 1, contentIndex = 0;

  // calculate the length of the content blob
  for ( ; it != limit; it++ )
  {
    contentLen += (FCULONG)(*it)->GetContent().size();
  }
  // update the size of the packet
  pktLen += (numPosts*sizeof(__FCPKT_FORUM_GET_THREAD_DETAILS_RESP::_thread_data));

  // allocate the packet
  d = (__FCPKT_FORUM_GET_THREAD_DETAILS_RESP*) new FCBYTE[pktLen];
  if ( d )
  {
    d->category_id = category_id;
    d->post_count = numPosts+1;
    d->contentSize = contentLen;
    // setup the parent post
    d->ThreadData[0].thread_id = pPost->GetID();
    d->ThreadData[0].parent_id = 0;
    d->ThreadData[0].author_id = pPost->GetAuthorID();
    strncpy( d->ThreadData[0].author_name, pPost->GetAuthorName().c_str(), sizeof(d->ThreadData[0].author_name) );
		strncpy( d->ThreadData[0].title, pPost->GetTitle().c_str(), sizeof(d->ThreadData[0].title) );
    strncpy( d->ThreadData[0].date_created, pPost->GetDateCreated().c_str(), sizeof(d->ThreadData[0].date_created) );
    d->ThreadData[0].contentIndex = contentIndex;
    d->ThreadData[0].contentLen = (FCULONG)pPost->GetContent().size();
    contentIndex += d->ThreadData[0].contentLen;

    // loop through any child posts/replies and add them to the ThreadData structure
    for ( it = posts.begin(); it != limit; it++, postIndex++ )
    {
      d->ThreadData[postIndex].thread_id = (*it)->GetID();
      d->ThreadData[postIndex].parent_id = (*it)->GetParentID();
      d->ThreadData[postIndex].author_id = (*it)->GetAuthorID();
      strncpy( d->ThreadData[postIndex].author_name, (*it)->GetAuthorName().c_str(), sizeof(d->ThreadData[postIndex].author_name) );
			strncpy( d->ThreadData[postIndex].title, (*it)->GetTitle().c_str(), sizeof(d->ThreadData[postIndex].title) );
      strncpy( d->ThreadData[postIndex].date_created, (*it)->GetDateCreated().c_str(), sizeof(d->ThreadData[postIndex].date_created) );
      d->ThreadData[postIndex].contentIndex = contentIndex;
      d->ThreadData[postIndex].contentLen = (FCULONG)(*it)->GetContent().size();
      contentIndex += d->ThreadData[postIndex].contentLen;
    }

    // send the packet
    PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_FORUM_GET_THREAD_DETAILS, ST_None);
    PEPacketHelper::SetPacketData(*pkt, 
                                  (void*)d, 
                                  pktLen);

    // send response to Client
    pkt->SetFieldValue("target", (void*)&clientSocket);
    QueuePacket(pkt, pRouter);

    // clear the data portion
    delete [] (FCBYTE*)d;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendForumThreadContentBlob(FCULONG category_id, ForumPost* pPost, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  if ( !pPost )
  {
    DYNLOG_ADDLOG("FCLogicWorld::SendForumThreadContentBlob() failed due to pPost == NULL");
    return;
  }

  PEPacket* pkt = new PEPacket;
  __FCPKT_FORUM_GET_THREAD_CONTENTBLOB_RESP* d = NULL;
  size_t pktLen = sizeof(__FCPKT_FORUM_GET_THREAD_CONTENTBLOB_RESP);
  const vector<ForumPost*>& posts = pPost->GetPosts();
  vector<ForumPost*>::const_iterator it = posts.begin();
  vector<ForumPost*>::const_iterator limit = posts.end();
  FCULONG numPosts = (FCULONG)posts.size(), contentLen = (FCULONG)pPost->GetContent().size(), postIndex = 1, contentIndex = 0;

  // calculate the length of the content blob
  for ( ; it != limit; it++ )
  {
    contentLen += (FCULONG)(*it)->GetContent().size();
  }
  // update the size of the packet
  pktLen += contentLen-1;

  d = (__FCPKT_FORUM_GET_THREAD_CONTENTBLOB_RESP*) new FCBYTE[pktLen];
  if ( d )
  {
    d->category_id = category_id;
    d->thread_id = pPost->GetID();
    d->contentSize = contentLen;

    // copy the main thread's content into the blob
    strncpy( (char*)&d->contentBlob[contentIndex], pPost->GetContent().c_str(), pPost->GetContent().size() );
    contentIndex += (FCULONG)pPost->GetContent().size();

    for ( it = posts.begin(); it != limit; it++ )
    {
      strncpy( (char*)&d->contentBlob[contentIndex], (*it)->GetContent().c_str(), (*it)->GetContent().size() );
      contentIndex += (FCULONG)(*it)->GetContent().size();
    }

    // send the packet
    PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_FORUM_GET_THREAD_CONTENTBLOB, ST_None);
    PEPacketHelper::SetPacketData(*pkt, 
                                  (void*)d, 
                                  pktLen);

    // send response to Client
    pkt->SetFieldValue("target", (void*)&clientSocket);
    QueuePacket(pkt, pRouter);

    // clear the data portion
    delete [] (FCBYTE*)d;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendForumCreateNewThreadResult(FCULONG category_id, FCULONG thread_id, bool bResult, BaseSocket* pRouter, FCSOCKET clientSocket)
{
	PEPacket* pkt = new PEPacket;
	__FCPKT_FORUM_CREATE_NEW_THREAD_RESP d;
	size_t pktLen = sizeof(__FCPKT_FORUM_CREATE_NEW_THREAD_RESP);

	d.category_id = category_id;
	d.thread_id = thread_id;
	d.bSuccess = bResult;

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_FORUM_CREATE_NEW_THREAD, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)&d, 
                                pktLen);

  // send response to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Mission send methods */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendMissionAcceptedResponse(Player* pPlayer, FCULONG mission_id, bool bSuccessFlag, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_MISSION_ACCEPT_RESP* d = NULL;
  size_t pktLen = sizeof(__FCPKT_MISSION_ACCEPT_RESP);
  Mission* pMission = NULL, *pChild = NULL;

  if ( (pMission = pPlayer->GetMission(mission_id)) )
  {
    size_t numChildren = pMission->GetChildCount();

    pktLen += (numChildren-1) * sizeof(__FCPKT_MISSION_ACCEPT_RESP::_child_missions);
    if ( (d = (__FCPKT_MISSION_ACCEPT_RESP*) new FCBYTE[pktLen]) )
    {
      d->bSuccessFlag = bSuccessFlag;
      d->mission_id = mission_id;
      d->numChildMissions = (FCULONG)numChildren;
      for ( size_t i = 0; i < numChildren; i++ )
      {
        if ( (pChild = pMission->GetChildMission(i)) )
          d->child_missions[i].mission_id = pChild->GetID();
      }

      // send the packet
      PEPacketHelper::CreatePacket(*pkt, FCPKT_RESPONSE, FCMSG_MISSION_ACCEPT, ST_None);
      PEPacketHelper::SetPacketData(*pkt, 
                                    (void*)d, 
                                    pktLen);

      // send response to Client
      pkt->SetFieldValue("target", (void*)&clientSocket);
      QueuePacket(pkt, pRouter);
    }
    else
    {
      DYNLOG_ADDLOG( "FCLogicWorld::SendMissionAcceptedResponse(): Failed to allocate memory for the packet" );
    }
  }
  else
  {
    DYNLOG_ADDLOG( "FCLogicWorld::SendMissionAcceptedResponse(): Failed to send response because accepted mission was not found in player's mission list" );
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendMissionComplete(FCULONG mission_id, BaseSocket* pRouter, FCSOCKET clientSocket)
{
  PEPacket* pkt = new PEPacket;
  __FCPKT_MISSION_COMPLETE d;

  d.mission_id = mission_id;

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_COMMAND, FCMSG_MISSION_COMPLETE, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)&d, 
                                sizeof(d));

  // send response to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Miscellaneous send methods */
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void SendXPGained(FCSHORT xpGained, FCULONG xpTotal, BaseSocket* pRouter, FCSOCKET clientSocket)
{
	PEPacket* pkt = new PEPacket;
	__FCPKT_XP_GAINED d;

	d.xpGained = xpGained;
	d.xpTotal = xpTotal;

  // send the packet
  PEPacketHelper::CreatePacket(*pkt, FCPKT_COMMAND, FCMSG_XP_GAINED, ST_None);
  PEPacketHelper::SetPacketData(*pkt, 
                                (void*)&d, 
                                sizeof(d));

  // send response to Client
  pkt->SetFieldValue("target", (void*)&clientSocket);
  QueuePacket(pkt, pRouter);
}