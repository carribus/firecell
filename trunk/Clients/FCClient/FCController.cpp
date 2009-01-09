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
		if ( m_pModel->ProcessData() )
    {
		  m_bRunning = m_pView->Update();
    }
    else
      break;
	}
}

///////////////////////////////////////////////////////////////////////

void FCController::OnModelEvent(FCModelEvent event)
{

}

///////////////////////////////////////////////////////////////////////
