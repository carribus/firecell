#include "../common/clienttypes.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "BankingWindow.h"

BankingWindow::BankingWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
, m_pAuthView(NULL)
, m_pBankView(NULL)
{
  m_appName = ResourceManager::instance().GetClientString(STR_APP_BANK_CAPTION);
}

///////////////////////////////////////////////////////////////////////

BankingWindow::~BankingWindow(void)
{
  if ( m_pAuthView )
    delete m_pAuthView;
  if ( m_pBankView )
    delete m_pBankView;
}

///////////////////////////////////////////////////////////////////////

bool BankingWindow::Create(s32 AppElemID, FCUINT optionID, std::wstring caption)
{
  bool bResult = InGameAppWindow::Create(AppElemID, optionID, SWT_APP_BANK, caption);
	core::rect<s32> wndRect, clientRect;

  // calculate the size of the window
  m_pDesktop->GetDesktopRect(wndRect);
  wndRect.UpperLeftCorner.X += wndRect.getWidth()/2;
  wndRect.UpperLeftCorner.Y += wndRect.getHeight()/4;
  wndRect.LowerRightCorner.Y = wndRect.UpperLeftCorner.Y + 200;

  // update the window
	if ( bResult )
	{
		m_pWindow->setRelativePosition(wndRect);
    CenterWindow();
		GetClientRect(clientRect);
  }

  // create the banking auth view
  m_pAuthView = new GUIBankAuthView(this, m_pEnv, clientRect, m_pWindow);
  m_pAuthView->setVisible(false);
  m_pAuthView->drop();
  // create the banking view
  m_pBankView = new GUIBankView(m_pEnv, clientRect, m_pWindow);
  m_pBankView->setVisible(true);
  m_pBankView->drop();

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

bool BankingWindow::OnBankConnected()
{
  m_pAuthView->setVisible(false);
  m_pBankView->setVisible(true);
  return true;
}

///////////////////////////////////////////////////////////////////////

bool BankingWindow::OnBankAuthNeeded()
{
  m_pBankView->setVisible(false);
  m_pAuthView->setVisible(true);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool BankingWindow::OnNoAccountExists()
{
  return true;
}

///////////////////////////////////////////////////////////////////////

bool BankingWindow::OnAccountDetailsUpdated(BankAccount* pAccount)
{
  if ( !pAccount )
    return false;

  if ( m_pBankView->isVisible() )
  {
    m_pBankView->updateAccountDetails(pAccount);
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

void BankingWindow::SendBankAuthentication(std::wstring& password)
{
  FCViewEventBankAuth e(password);
  m_pController->OnViewEvent(e);
}