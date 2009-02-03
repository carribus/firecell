#include "ForumWindow.h"

ForumWindow::ForumWindow(Desktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
{
}

///////////////////////////////////////////////////////////////////////

ForumWindow::~ForumWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ForumWindow::Create(FCUINT optionID, std::wstring caption)
{
  bool bResult = InGameAppWindow::Create(optionID, DOT_Forum, caption);
	core::rect<s32> wndRect(0, 0, 640, 480), clientRect;

	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
    CenterWindow();
		GetClientRect(clientRect);
  }

  return bResult;
}