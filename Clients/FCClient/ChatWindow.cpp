#include <sstream>
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
    createTab(L"Channels");
  }

  // At this point, we want to 'connect' to the chat server
  addToChatLog(L"Server", L"Connecting to chat server...\n");
  RequestChatServerConnect();

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::OnChatConnected()
{
  addToChatLog(L"Server", L"Connected to chat server!\n");
  addToChatLog(L"Server", L"Requesting available channels...\n");
  RequestChatRoomList();
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::OnChatChannelsUpdated()
{
  std::wstringstream ss;
  ChatModel& cm = ChatModel::instance();

  ss << cm.getChannelCount() << L" channels found\n";
  addToChatLog(L"Server", ss.str());

  // iterate over the channel map
  const ChatModel::ChannelMap& channels = cm.getChannelMap();
  ChatModel::ChannelMap::const_iterator it = channels.begin();
  ChatModel::ChannelMap::const_iterator limit = channels.end();

  for ( ; it != limit; ++it )
  {
    ss.str(L"");
    ss << it->second->getName().c_str() << "\n";
    addToChatLog(L"Channels", ss.str());
  }
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::RequestChatServerConnect()
{
  FCViewEvent e(VE_ChatConnect);
  
  SetWaitingForResponse(true);
  m_pController->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::RequestChatRoomList()
{
  FCViewEvent e(VE_ChatListChannels);

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
    m_panes[label] = new GUIChatPane( m_pDevice->getGUIEnvironment(), rect, pTab );
  }

  return pTab;
}

///////////////////////////////////////////////////////////////////////

void ChatWindow::addToChatLog(std::wstring pane, std::wstring str)
{
  ChatPaneMap::iterator it = m_panes.find(pane);
  GUIChatPane* pPane = NULL;
  IGUITab* pTab = NULL;

  if ( it != m_panes.end() )
  {
    pPane = (*it).second;
    if ( pPane )
    {
      pTab = static_cast<IGUITab*>(pPane->getParent());
      if ( pTab )
        pPane->addLogItem(str.c_str());
    }
  }
}