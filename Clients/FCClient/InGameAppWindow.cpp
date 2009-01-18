#include "InGameAppWindow.h"

InGameAppWindow::InGameAppWindow(FCController* pController, IGUIEnvironment* pEnv)
: m_pEnv(pEnv)
, m_pController(pController)
, m_pWindow(NULL)
, m_optionID(0)
, m_bWaitingForResponse(false)
{
	m_pEnv->grab();
}

///////////////////////////////////////////////////////////////////////

InGameAppWindow::~InGameAppWindow(void)
{
	m_pEnv->drop();
}

///////////////////////////////////////////////////////////////////////

bool InGameAppWindow::Create(FCUINT optionID, DesktopOptionType type, std::wstring caption)
{
	if ( !m_pEnv || !m_pController )
		return false;

	if ( !(m_pWindow = m_pEnv->addWindow( core::rect<s32>(0, 0, 0, 0), false, caption.c_str() )) )
		return false;

	m_type = type;
	m_optionID = optionID;
		
	return true;
}

///////////////////////////////////////////////////////////////////////

void InGameAppWindow::GetClientRect(core::rect<s32>& rect)
{
	rect = m_pWindow->getAbsoluteClippingRect();
	rect.UpperLeftCorner.X += 2;
	rect.UpperLeftCorner.Y += 19;
	rect.LowerRightCorner.X -= 2;
	rect.LowerRightCorner.Y -= 2;
}