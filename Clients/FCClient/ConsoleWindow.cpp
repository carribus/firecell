#include "FCController.h"
#include "FCViewEvent.h"
#include "ConsoleWindow.h"

#define CWID_STATIC_LOG			1

ConsoleWindow::ConsoleWindow(FCController* pController, IGUIEnvironment* pEnv)
: InGameAppWindow(pController, pEnv)
, m_pLogWnd(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ConsoleWindow::~ConsoleWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ConsoleWindow::Create(FCUINT optionID, std::wstring caption)
{
	bool bResult = InGameAppWindow::Create(optionID, DOT_Console, caption);
	core::rect<s32> wndRect(0, 0, 500, 400), clientRect;

	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
		GetClientRect(clientRect);
		m_pLogWnd = m_pEnv->addStaticText(L"", clientRect, false, false, m_pWindow, CWID_STATIC_LOG);
		m_pLogWnd->setBackgroundColor( video::SColor(255, 0, 0, 0) );
		m_pLogWnd->setOverrideColor( video::SColor(255, 0, 196, 0) );
		m_pLogWnd->setOverrideFont( m_pEnv->getSkin()->getFont() );

		// request a refresh
		FCViewEvent e(VE_ConRefresh);
		m_pController->OnViewEvent(e);
	}

	return bResult;
}	
