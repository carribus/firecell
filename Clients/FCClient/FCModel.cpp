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
#include "../common/ResourceManager.h"
#include "Settings.h"
#include "FCModel.h"

FCModel::FCModel(void)
: m_connectRetry(1)
{
	m_state.state = FCModel::None;
	m_state.stateStep = 0;
}

///////////////////////////////////////////////////////////////////////

FCModel::~FCModel(void)
{
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

	vector<IModelEventSink*>::iterator it = m_sinks.begin();

	while ( it != m_sinks.end() )
	{
		(*it)->OnModelEvent(e);
		it++;
	}
}

///////////////////////////////////////////////////////////////////////

bool FCModel::ProcessData()
{
	bool bResult = true;

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
    m_connectRetry = 0;
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
  if ( m_state.state != FCModel::ShuttingDown )
  {
    // if we are not shutting down, then we should probably try to reconnect...
    ConnectToServer();
  }
}

///////////////////////////////////////////////////////////////////////

void FCModel::OnDataReceived(BaseSocket* pSocket, FCBYTE* pData, int nLen)
{
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