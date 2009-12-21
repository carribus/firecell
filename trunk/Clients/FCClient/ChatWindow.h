#ifndef _CHATWINDOW_H_
#define _CHATWINDOW_H_

#include <map>
#include <string>
#include <irrlicht.h>
#include "GUIChatPane.h"
#include "InGameAppWindow.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class ChatWindow : public InGameAppWindow
{
public:
  ChatWindow(IDesktop* pDesktop, FCController* pController, IrrlichtDevice* pDevice);
  ~ChatWindow(void);

  bool Create(s32 AppElemID, FCUINT optionID, std::wstring caption);

  void OnChatConnected();

private:

  void RequestChatServerConnect();
  void RequestChatRoomList();

  IGUITab* createTab(std::wstring label);
  void addToChatLog(std::wstring pane, std::wstring str);

  /*
   *  Private Members
   */
  typedef std::map< std::wstring, GUIChatPane* > ChatPaneMap;

  IrrlichtDevice*         m_pDevice;
  IGUITabControl*         m_pTabCtrl;
  ChatPaneMap             m_panes;
};

#endif//_CHATWINDOW_H_