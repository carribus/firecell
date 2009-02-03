#include "InGameAppWindow.h"

InGameAppWindow::InGameAppWindow(Desktop* pDesktop, FCController* pController, IGUIEnvironment* pEnv)
: m_pEnv(pEnv)
, m_pController(pController)
, m_pDesktop(pDesktop)
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

bool InGameAppWindow::Create(FCUINT optionID, DesktopOptionType type, std::wstring caption, bool bModal)
{
	if ( !m_pEnv || !m_pController )
		return false;

	if ( !(m_pWindow = m_pEnv->addWindow( core::rect<s32>(0, 0, 0, 0), bModal, caption.c_str(), (IGUIElement*)m_pDesktop )) )
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
  core::rect<s32> absRect = m_pWindow->getAbsolutePosition();
	rect.UpperLeftCorner.X = 3;
  rect.UpperLeftCorner.Y = 21;
  rect.LowerRightCorner.X = rect.UpperLeftCorner.X + absRect.getWidth() - 6;
  rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + absRect.getHeight() - 24;
}

///////////////////////////////////////////////////////////////////////

void InGameAppWindow::CenterWindow()
{
  core::dimension2d<s32> screenSize = m_pEnv->getVideoDriver()->getScreenSize();
  core::rect<s32> wndSize = m_pWindow->getAbsolutePosition();
  core::position2d<s32> newWndPos;

  newWndPos.X = (screenSize.Width / 2) - wndSize.getWidth()/2;
  newWndPos.Y = (screenSize.Height / 2) - wndSize.getHeight()/2;
  m_pWindow->move(newWndPos);
}