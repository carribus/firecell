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
#include <vector>
#include "../../common/protocol/fcprotocol.h"
#include "../common/ResourceManager.h"
#include "Settings.h"
#include "FCModel.h"

FCModel* FCModel::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

FCModel::FCModel(void)
: m_connectRetry(1)
, m_pCharacter(NULL)
{
	m_state.state = FCModel::None;
	m_state.stateStep = 0;
}

///////////////////////////////////////////////////////////////////////

FCModel::~FCModel(void)
{
	if ( m_pCharacter )
		delete m_pCharacter;
}

///////////////////////////////////////////////////////////////////////

FCModel& FCModel::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new FCModel;
  }
  
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void FCModel::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

void FCModel::SubscribeToEvents(IModelEventSink* pSink)
{
	if ( !IsSubscribed(pSink) )
		m_sinks.push_back(pSink);
}

///////////////////////////////////////////////////////////////////////

bool FCModel::IsSubscribed(IModelEventSink* pSink)
{
	vector<IModelEventSink*>::iterator it = m_sinks.begin();

	while ( it != m_sinks.end() )
	{
		if ( (*it) == pSink )
			return true;
		it++;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::Initialise()
{
	// create the socket
	if ( !m_sock.Create() )
	{
		// log an error
		return false;
	}
	m_sock.Subscribe(this);
	
	// configure the server object
	m_server.SetSocket(&m_sock);
	m_extractor.Prepare( __FCPACKET_DEF );

	SetState( FCModel::Loading );

	return true;
}

///////////////////////////////////////////////////////////////////////

void FCModel::SetState(e_ModelState newState)
{
	m_state.state = newState;
	m_state.stateStep = 0;
	FireEvent(FCME_StateChange, (void*)newState);
}

///////////////////////////////////////////////////////////////////////

void FCModel::SetStateStep(FCSHORT stateStep)
{
  m_state.stateStep = stateStep;
  FireEvent(FCME_StateChange, (void*) m_state.state);
}

///////////////////////////////////////////////////////////////////////

void FCModel::FireEvent(e_FCEventType type, void* pData)
{
	FCModelEvent e(type, (long long)pData);

  m_mutexSinks.Lock();
	vector<IModelEventSink*>::iterator it = m_sinks.begin();

	while ( it != m_sinks.end() )
	{
		(*it)->OnModelEvent(e);
		it++;
	}
  m_mutexSinks.Unlock();
}

///////////////////////////////////////////////////////////////////////

void FCModel::ProcessIncomingData()
{
  FCBYTE type;
	DataQueueItem dqi;
	bool bContinue = false;

	m_mutexDataIn.Lock();
	bContinue = GetNextQueueItem(m_qDataIn, dqi);
	m_mutexDataIn.Unlock();

	while ( bContinue )
	{
    switch ( dqi.type )
    {
    case  DataQueueItem::DQI_DataIn:
      {
        if ( dqi.pPkt->GetField("type", &type, sizeof(FCBYTE)) )
        {
          switch ( type )
          {
          case  FCPKT_COMMAND:
            OnCommand(dqi.pPkt, dqi.pSocket);
            break;

          case  FCPKT_RESPONSE:
            OnResponse(dqi.pPkt, dqi.pSocket);
            break;

          case  FCPKT_ERROR:
            OnError(dqi.pPkt, dqi.pSocket);
            break;
          }
        }
      }
      break;

    case  DataQueueItem::DQI_Connect:
      {
		    SetStateStep( MS_Connecting_Connected );

        switch ( m_state.state )
        {
        case  FCModel::Connecting:
          // only move over to the login IF we are currently in the connecting state...
          SetState( FCModel::Login );
          break;

        case  FCModel::Login:
          break;

        case  FCModel::Playing:
          break;

        default:
          break;
        }
      }
      break;

    case  DataQueueItem::DQI_Disconnect:
      {
        if ( m_state.state != FCModel::ShuttingDown )
        {
          // if we are not shutting down, then we should probably try to reconnect...
          ConnectToServer();
        }
      }
      break;
    }

    m_mutexDataIn.Lock();
    bContinue = GetNextQueueItem(m_qDataIn, dqi);
    m_mutexDataIn.Unlock();
	}
}

///////////////////////////////////////////////////////////////////////

bool FCModel::GetNextQueueItem(DataQueue& queue, DataQueueItem& dest)
{
  dest.pPkt = NULL;
  dest.pSocket = NULL;

  if ( queue.size() )
  {
    dest = queue.front();
    queue.pop();
    return true;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponse(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;
  bool bHandled = false;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));

  switch ( msgID )
  {
  case  FCMSG_INFO_SERVER:
    {
      bHandled = OnResponseServiceInfo(pPkt, pSocket);
    }
    break;

  case  FCMSG_LOGIN:
    {
      bHandled = OnResponseLogin(pPkt, pSocket);
    }
    break;

  case  FCMSG_GETCHARACTERS:
    {
      bHandled = OnResponseGetCharacters(pPkt, pSocket);
    }
    break;

  case  FCMSG_SELECT_CHARACTER:
    {
      bHandled = OnResponseSelectCharacter(pPkt, pSocket);
    }
    break;

	case  FCMSG_CHARACTER_ASSET_REQUEST:
    {
      bHandled = OnResponseCharacterAssetRequest(pPkt, pSocket);
    }
    break;

  case  FCMSG_GET_DESKTOP_OPTIONS:
    {
      bHandled = OnResponseGetDesktopOptions(pPkt, pSocket);
    }
    break;

	case	FCMSG_ACTIVATE_DESKTOP_OPTION:
		{
			bHandled = OnResponeActivateDesktopOptions(pPkt, pSocket);
		}
		break;

	case	FCMSG_CON_GET_FS_INFO:
		{
			bHandled = OnResponseConsoleGetFileSystemInfo(pPkt, pSocket);
		}
		break;

	case	FCMSG_CON_COMMAND:
		{
			bHandled = OnResponseConsoleCommand(pPkt, pSocket);
		}
		break;

  default:

    if ( !bHandled )
    {
      printf("Unknown Response packet received (id:%ld)\n", msgID);
      pPkt->DebugDump();
    }
    break;
  }

  return bHandled;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseServiceInfo(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_INFO_SERVER d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseLogin(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_LOGIN_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);
  switch ( d.loginStatus )
  {
  case  LoginFailed:
		SetStateStep( FCModel::MS_Login_LoginFailed );
		SetState( FCModel::ShuttingDown );
    break;

  case  LoginSuccess:
		SetStateStep( FCModel::MS_Login_LoginSucceeded );
    m_server.RequestCharacterInfo();
    break;

  case  LoginAccountLoggedInAlready:
		SetStateStep( FCModel::MS_Login_LoginFailed_AccountInUse );
		SetState( FCModel::ShuttingDown );
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseGetCharacters(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CHARACTER_LIST d;
  size_t dataLen;

  // clear the character array of any information it may be holding
  m_characters.clear();

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

  // read the characters into the model and make the data available to the view
  if ( d.numCharacters > 0 )
  {   
    for ( FCBYTE i = 0; i < d.numCharacters; i++ )
    {
      wstringstream ss;
      ss << d.characters[i].name;
      Character c(d.characters[i].character_id, ss.str());
      c.SetXP( d.characters[i].xp );
      c.SetLevel( d.characters[i].level );
      c.SetFameScale( d.characters[i].fame_scale );
      c.SetCountryID( d.characters[i].country_id );
      c.SetCityID( d.characters[i].city_id );

      m_characters.push_back(c);
    }
	}
  else
  {
    // no characters found, take a stand :D
  }

	SetState( FCModel::CharacterSelection );

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseSelectCharacter(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_SELECT_CHARACTER_RESP d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);

	// select the character
	bool bFound = false;
	vector<Character>::iterator it = m_characters.begin();
	for ( ; it != m_characters.end(); it++ )
	{
		if ( it->GetID() == d.character_id )
		{
			m_pCharacter = new Character(it->GetID(), it->GetName());
			bFound = true;
			break;
		}
	}

  if ( d.status == CharacterSelectSucceeded && bFound )
  {
    m_server.SendCharacterAssetRequest(m_pCharacter->GetID());
  }
  else
  {
		SetStateStep( MS_CharacterSelection_Failed );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseCharacterAssetRequest(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CHARACTER_ASSET_REQUEST_RESP d;
  size_t dataLen;


  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  pPkt->GetField("data", &d, dataLen);
/*
  printf("\nIP Address: %ld.%ld.%ld.%ld\n", d.ip_address.a, d.ip_address.b, d.ip_address.c, d.ip_address.d);
  printf("Computer Specifications:\n\n");
  printf("\tName: %s\n\tHDD Size: %ld MB\n\tNetwork Speed: %ld MBits\n",
         d.computer.name, d.computer.hddSize, d.computer.networkSpeed);
  printf("\tProcessor:\n\t\tName: %s\n\t\tCore Count: %ld\n\t\tCore Speed: %ldMhz\n",
         d.computer.processor.name, d.computer.processor.core_count, d.computer.processor.core_speed);
  printf("\tOperating System:\n\t\tName: %s\n\t\tKernal: %s\n",
         d.computer.os.name, d.computer.os.kernel_name);
  printf("\tMemory Module:\n\t\tName: %s\n\t\tSize: %ldMB\n",
         d.computer.memory.name, d.computer.memory.mem_size);
*/
  m_server.RequestDesktopOptions(m_pCharacter->GetID());

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseGetDesktopOptions(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_GET_DESKTOP_OPTIONS_RESP* d;
  DesktopOption option;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_GET_DESKTOP_OPTIONS_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

  for ( int i = 0; i < d->numOptions; i++ )
  {
    option.optionID = d->Options[i].optionID;
    option.type = d->Options[i].type;
    strncpy(option.name, d->Options[i].name, sizeof(option.name));

    m_desktopOptions[option.optionID] = option;
  }

	SetState( FCModel::Playing );
	
	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponeActivateDesktopOptions(PEPacket* pPkt, BaseSocket* pSocket)
{
	__FCPKT_ACTIVATE_DESKTOP_OPTION_RESP d;
	size_t dataLen;

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	pPkt->GetField("data", &d, sizeof(d));

	// check if we can open the option
	if ( d.canActivate )
	{
		FireEvent(FCME_OpenApplication, (void*)&d);
	}
	else
	{
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseConsoleGetFileSystemInfo(PEPacket* pPkt, BaseSocket* pSocket)
{
	__FCPKT_CON_GET_FS_INFO_RESP d;
	size_t dataLen;

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	pPkt->GetField("data", &d, sizeof(d));
	
  m_fsInfo.dirSeperator = d.dirSeperator;
  m_fsInfo.fsStyle = (FCBYTE) d.fsStyle & 0x000000FF;
  m_fsInfo.currentDir = d.currentDir;

	FireEvent(FCME_Console_FileSystemInfo, (void*)&m_fsInfo);

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnResponseConsoleCommand(PEPacket* pPkt, BaseSocket* pSocket)
{
  __FCPKT_CON_COMMAND_RESP* d;
  size_t dataLen;

  pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
  d = (__FCPKT_CON_COMMAND_RESP*) new FCBYTE[ dataLen ];
  pPkt->GetField("data", d, dataLen);

	FireEvent(FCME_Console_Command, (void*)d);

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::OnError(PEPacket* pPkt, BaseSocket* pSocket)
{
  if ( !pPkt || !pSocket )
    return false;

  FCSHORT msgID;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));


  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::ProcessData()
{
	bool bResult = true;

	// handle outstanding incoming data
	ProcessIncomingData();

	// handle the model's state
	switch ( m_state.state )
	{
	case	FCModel::Loading:
		LoadResources();
    SetState( FCModel::Connecting );
		break;

	case	FCModel::Connecting:
		if ( m_state.stateStep == FCModel::MS_Connecting_None )
			ConnectToServer();
		break;

	case	FCModel::CharacterSelection:
		{

		}
		break;

	case	FCModel::Login:
		break;

	case	FCModel::Playing:
		break;

	case	FCModel::ShuttingDown:
    if ( m_state.stateStep == FCModel::MS_ShuttingDown_None )
    {
      Shutdown();
      bResult = false;        // cause the controller to exit from its loop
    }
		break;
	}

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void FCModel::OnAccepted(BaseSocket* pSocket, int nErrorCode)
{
}

///////////////////////////////////////////////////////////////////////

void FCModel::OnConnected(BaseSocket* pSocket, int nErrorCode)
{
	m_bConnected = (nErrorCode == 0);
	if ( m_bConnected )
	{
    DataQueueItem dqi;

    m_connectRetry = 0;

    dqi.pSocket = pSocket;
    dqi.pPkt = NULL;
    dqi.type = DataQueueItem::DQI_Connect;

		m_mutexDataIn.Lock();
    m_qDataIn.push(dqi);
    m_mutexDataIn.Unlock();
	}
	else
	{
    if ( m_connectRetry < (short)atoi( Settings::instance().GetValue("FCClient/Settings/ConnectAttempts", "count").c_str() ) )
    {
      SetStateStep( MS_Connecting_Retry );
      SetStateStep( MS_Connecting_None );
      m_connectRetry++;
    }
    else
    {
      SetStateStep( MS_Connecting_FinalFail );
      // fire a shut down event to let our subscribers clean up before we do
      SetState(FCModel::ShuttingDown);
    }
	}
}

///////////////////////////////////////////////////////////////////////

void FCModel::OnDisconnected(BaseSocket* pSocket, int nErrorCode)
{
	m_bConnected = false;
  DataQueueItem dqi;

  dqi.type = DataQueueItem::DQI_Disconnect;
  dqi.pPkt = NULL;
  dqi.pSocket = pSocket;

	m_mutexDataIn.Lock();
  m_qDataIn.push(dqi);
  m_mutexDataIn.Unlock();
}

///////////////////////////////////////////////////////////////////////

void FCModel::OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen)
{
  PEPacket* pPkt = NULL;
  size_t offset = 0;
  DataQueueItem dqi;

  while ( (pPkt = m_extractor.Extract((const char*)pData, offset, (size_t)nLen)) )
  {
    dqi.pSocket = pSocket;
    dqi.pPkt = pPkt;
    dqi.type = DataQueueItem::DQI_DataIn;

		m_mutexDataIn.Lock();
    m_qDataIn.push(dqi);
    m_mutexDataIn.Unlock();
  }
}

///////////////////////////////////////////////////////////////////////

void FCModel::StartLogin(wstring username, wstring password)
{
	char un[128], pw[128];

	sprintf(un, "%S", username.c_str());
	sprintf(pw, "%S", password.c_str());
	m_server.Login(un, pw);
}

///////////////////////////////////////////////////////////////////////

void FCModel::SelectCharacter(FCUINT characterID)
{
	m_server.SendCharacterSelection(characterID);
	SetStateStep( FCModel::MS_CharacterSelection_CharacterSelected );
}

///////////////////////////////////////////////////////////////////////

void FCModel::ActivateDesktopOption(FCULONG optionID)
{
	m_server.RequestDesktopOptionActivate(optionID);
}

///////////////////////////////////////////////////////////////////////

void FCModel::ConsoleRefresh()
{
	m_server.RequestCharacterFileSystemInfo( m_pCharacter->GetID() );
}

///////////////////////////////////////////////////////////////////////

void FCModel::ConsoleCommandIssued(string cmd, string curdir)
{
	m_server.SendConsoleCommand(curdir, cmd);
}

///////////////////////////////////////////////////////////////////////

bool FCModel::LoadResources()
{
  // Load the client settings
	if ( !Settings::instance().LoadSettings("./clientdata/settings.xml") )
	{
		fprintf(stderr, "Failed to load client settins\n");
		return false;
	}

  // load the client strings
  // TODO: Make this a load based on language (currently EN is hardcoded)
	if ( ResourceManager::instance().LoadClientStrings("./clientdata/strings_en.xml") == -1 )
	{
    fprintf(stderr, "Failed to load client strings\n");
    return false;
	}

	SetStateStep( (FCSHORT)FCModel::MS_Loading_Text );

	// load the mission text
  // TODO: Make this multi-language enabled (again, EN is hardcoded)
  if ( ResourceManager::instance().LoadMissionStrings("./clientdata/missions/missions_en.xml") == -1 )
  {
    fprintf(stderr, "Failed to load mission strings\n");
    return false;
  }

  // fire off an event allowing the view to load its graphical assets
  SetStateStep( (FCSHORT)FCModel::MS_Loading_Graphics );

  // fire off an event so that the necessary sounds can get loaded
  SetStateStep( (FCSHORT)FCModel::MS_Loading_Sounds );

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::ConnectToServer()
{
	string server = Settings::instance().GetValue("FCClient/Settings/Server", "address");
	int port = atoi( Settings::instance().GetValue("FCClient/Settings/Server", "port").c_str() );

	SetStateStep(MS_Connecting_Connecting);

	m_sock.Connect(server.c_str(), (short)port);

	return true;
}

///////////////////////////////////////////////////////////////////////

void FCModel::Shutdown()
{
  SetStateStep(FCModel::MS_ShuttingDown_InProgress);
  // close the socket
  if ( m_bConnected )
  {
    m_sock.Disconnect();
  }
}