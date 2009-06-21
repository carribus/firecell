#include "../common/clienttypes.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "BankingWindow.h"

BankingWindow::BankingWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
, m_pBankView(NULL)
, m_pModel(NULL)
{
  m_pModel = BankModel::instance();
  m_appName = ResourceManager::instance().GetClientString(STR_APP_BANK_CAPTION);
}

///////////////////////////////////////////////////////////////////////

BankingWindow::~BankingWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool BankingWindow::Create(s32 AppElemID, FCUINT optionID, std::wstring caption)
{
  bool bResult = InGameAppWindow::Create(AppElemID, optionID, SWT_APP_FORUM, caption);
	core::rect<s32> wndRect, clientRect;

  // calculate the size of the window
  m_pDesktop->GetDesktopRect(wndRect);
  wndRect.UpperLeftCorner.X += wndRect.getWidth()/4;
  wndRect.UpperLeftCorner.Y += wndRect.getHeight()/4;

  // update the window
	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
    CenterWindow();
		GetClientRect(clientRect);
  }

  // create the banking view
  m_pBankView = new GUIBankView(m_pEnv, clientRect, m_pWindow);
  m_pBankView->setVisible(true);
  m_pBankView->drop();

  m_pEnv->setFocus( m_pBankView );

  ConnectToBank();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void BankingWindow::ConnectToBank()
{
  FCViewEvent e(VE_BankConnect);
  
  SetWaitingForResponse(true);
  m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

