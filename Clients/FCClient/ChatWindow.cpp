#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "../common/clienttypes.h"
#include "ChatModel.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice)
: InGameAppWindow(pDesktop, pController, pDevice->getGUIEnvironment())
, m_pDevice(pDevice)
, m_pTabCtrl(NULL)
{
  m_appName = ResourceManager::instance().GetClientString(STR_APP_CHAT_CAPTION);
}

///////////////////////////////////////////////////////////////////////

ChatWindow::~ChatWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ChatWindow::Create(s32 AppElemID, FCUINT optionID, std::wstring caption)
{
  bool bResult = InGameAppWindow::Create(AppElemID, optionID, SWT_APP_CHAT, caption);
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

  // create the main tab control
	GetClientRect(clientRect);
  if ( (m_pTabCtrl = m_pDevice->getGUIEnvironment()->addTabControl(clientRect, m_pWindow, true)) )
  {
    m_pTabCtrl->setTabHeight(22);
    m_pTabCtrl->setTabVerticalAlignment( EGUIA_LOWERRIGHT );
    createTab(L"Server");
  }

  // At this point, we want to 'connect' to the chat server
  RequestChatServerConnect();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::RequestChatServerConnect()
{
  FCViewEvent e(VE_ChatConnect);
  
  SetWaitingForResponse(true);
  m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

IGUITab* ChatWindow::createTab(std::wstring label)
{
  IGUITab* pTab = NULL;

  if ( m_pTabCtrl )
  {
    core::rect<s32> rect;
    GetClientRect(rect);

    rect.LowerRightCorner.X -= rect.UpperLeftCorner.X;
    rect.LowerRightCorner.Y -= rect.UpperLeftCorner.Y;
    rect.UpperLeftCorner.X -= rect.UpperLeftCorner.X;
    rect.UpperLeftCorner.Y -= rect.UpperLeftCorner.Y;

    pTab = m_pTabCtrl->addTab(label.c_str());
    new GUIChatPane( m_pDevice->getGUIEnvironment(), rect, pTab );
  }

  return pTab;
}