#include "InGameAppWindow.h"

InGameAppWindow::InGameAppWindow(FCController* pController, IGUIEnvironment* pEnv)
: m_pEnv(pEnv)
, m_pController(pController)
, m_pWindow(NULL)
, m_optionID(0)
, m_bWaitingForResponse(false)
{
}

///////////////////////////////////////////////////////////////////////

InGameAppWindow::~InGameAppWindow(void)
{
	if ( m_pWindow )
		m_pWindow->remove();
}

///////////////////////////////////////////////////////////////////////

bool InGameAppWindow::Create(FCUINT optionID, DesktopOptionType type, std::wstring caption)
{
	if ( !m_pEnv || !m_pController )
		return false;

	if ( !(m_pWindow = m_pEnv->addWindow( core::rect<s32>(0, 0, 0, 0), false, caption.c_str() )) )
		return false;
	m_pWindow->grab();

  IGUIButton* pBtn = m_pWindow->getMaximizeButton();
  pBtn->setVisible(true);
  pBtn = m_pWindow->getMinimizeButton();
  pBtn->setVisible(true);

	m_type = type;
	m_optionID = optionID;
		
	return true;
}

///////////////////////////////////////////////////////////////////////

void InGameAppWindow::GetClientRect(core::rect<s32>& rect)
{
	rect = m_pWindow->getAbsoluteClippingRect();
	rect.UpperLeftCorner.X += 3;
	rect.UpperLeftCorner.Y += 21;
	rect.LowerRightCorner.X -= 3;
	rect.LowerRightCorner.Y -= 3;
}