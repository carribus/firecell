#include "../common/ResourceManager.h"
#include "FCController.h"

FCController::FCController(void)
: m_pModel(NULL)
, m_pView(NULL)
, m_bRunning(false)
{
}

///////////////////////////////////////////////////////////////////////

FCController::~FCController(void)
{
}

///////////////////////////////////////////////////////////////////////

void FCController::SetModel(FCModel* pModel)
{
	m_pModel = pModel;
	pModel->SubscribeToEvents(this);
}

///////////////////////////////////////////////////////////////////////

void FCController::SetView(FCView* pView)
{
	m_pView = pView;
	pView->SetController(this);
}

///////////////////////////////////////////////////////////////////////

bool FCController::Initialise()
{
	if ( !m_sock.Create() )
	{
		// log an error
		return false;
	}

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

	if ( m_pModel )
		m_pModel->SetState( FCModel::Loading );

	return true;
}

///////////////////////////////////////////////////////////////////////

void FCController::Run()
{
	if ( !m_pModel || !m_pView )
		return;

	m_bRunning = true;
	while ( m_bRunning )
	{
		this->Process();
		m_bRunning = m_pView->Update();
	}
}

///////////////////////////////////////////////////////////////////////

void FCController::OnModelEvent(FCModelEvent event)
{

}

///////////////////////////////////////////////////////////////////////

void FCController::Process()
{
}