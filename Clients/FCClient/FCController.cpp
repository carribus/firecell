#include "FCController.h"

FCController::FCController(void)
: m_pModel(NULL)
{
}

FCController::~FCController(void)
{
}

void FCController::SetModel(FCModel* pModel)
{
	m_pModel = pModel;
	pModel->SubscribeToEvents(this);
}

void FCController::SetView(FCView* pView)
{
	m_pView = pView;
	pView->SetController(this);
}
