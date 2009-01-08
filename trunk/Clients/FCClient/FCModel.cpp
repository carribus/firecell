#include "../common/ResourceManager.h"
#include "FCModel.h"

FCModel::FCModel(void)
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
    SetState( FCModel::Login );
		break;

	case	FCModel::Login:
		break;

	case	FCModel::Playing:
		break;

	case	FCModel::ShuttingDown:
		break;
	}

	return bResult;
}

///////////////////////////////////////////////////////////////////////

bool FCModel::LoadResources()
{
  SetStateStep( (FCSHORT)FCModel::MS_Loading_Text );

	if ( ResourceManager::instance().LoadClientStrings("./clientdata/strings_en.xml") == -1 )
	{
    fprintf(stderr, "Failed to load client strings\n");
    return false;
	}

  if ( ResourceManager::instance().LoadMissionStrings("./clientdata/missions/missions_en.xml") == -1 )
  {
    fprintf(stderr, "Failed to load mission strings\n");
    return false;
  }

  SetStateStep( (FCSHORT)FCModel::MS_Loading_Graphics );

  SetStateStep( (FCSHORT)FCModel::MS_Loading_Sounds );

	return true;
}