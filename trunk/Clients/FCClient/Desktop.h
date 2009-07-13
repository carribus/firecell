#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include <vector>
#include <irrlicht.h>
#include "../../common/game_objects/ItemSoftware.h"
#include "../../common/PThreadMutex.h"
#include "IDesktop.h"
#include "DesktopAppBar.h"
#include "DesktopIcon.h"
#include "InGameAppWindow.h"
#include "SoftwareMgrWindow.h"

using namespace irr;
using namespace video;

class ViewLogicGame;

class Desktop : public IGUIElement
              , public IDesktop
{
public:
  /*
   *  Structure representing a desktop option
   */
  struct DesktopOption
	{
    FCULONG optionID;
    FCUINT type;
    wstring name;
		ITexture* pTexture;
		core::rect<s32> rect;
		bool isHighlighted;
	};

  Desktop(ViewLogicGame& owner, IrrlichtDevice* pDevice);
  ~Desktop(void);

  bool Create();
  void Draw();
	bool OnEvent(const SEvent& event);

  bool AddApplication(ItemSoftware* pSoftware);
  bool OpenApplication(FCULONG itemID, FCSHORT cpuCost, FCULONG memCost);
	bool IsApplicationRunning(FCUINT appType);

  void ShowError(const wchar_t* caption, const wchar_t* text, bool bModal = true, int flags = EMBF_OK);

	bool OnGUIEvent(SEvent::SGUIEvent event);
  bool OnConsoleEvent(FCModelEvent& event);
	bool OnForumEvent(FCModelEvent& event);
	bool OnMissionEvent(FCModelEvent& event);
  bool OnBankEvent(FCModelEvent& event);
  bool OnSoftwareEvent(FCModelEvent& event);

  /*
   *  IDesktop implementation
   */
	void OnDesktopIconSelected(DesktopIcon* pIcon);
  void OnDesktopIconActivated(DesktopIcon* pIcon);
  void OnApplicationActivated(InGameAppWindow* pApp);
  void GetDesktopRect(core::rect<s32>& rect);

private:

  /*
   *  Private Methods
   */
  void UpdateDesktopOptions();
  void updateIconPositions();

	InGameAppWindow* GetAppWindowByType(FCULONG type);
  FCDialog* GetUtilityWindow(IGUIElement* pWnd);
  void CloseUtilityWindow(IGUIElement* pWnd);

  void OpenSoftwareManagerWindow();

  /*
   *  Event Handlers
   */
  bool OnMouseLButtonDown(irr::SEvent::SMouseInput event);
  bool OnMouseMoved(irr::SEvent::SMouseInput event);
  bool OnGUIElementFocused(irr::SEvent::SGUIEvent event);
  bool OnGUIElementClosed(irr::SEvent::SGUIEvent event);
  bool OnGUIMenuItemSelected(irr::SEvent::SGUIEvent event);

  /*
   *  Private Data Members
   */
  ViewLogicGame&              m_owner;
  IrrlichtDevice*             m_pDevice;
	ITexture*										m_pBackground;
	IGUIFont*										m_pFontCourier;

  DesktopAppBar*              m_pAppBar;

  /*
   *  Desktop option container and utility members
   */
  typedef std::map<FCUINT, DesktopIcon*> DesktopIconMap;
  DesktopIconMap              m_mapDesktopIcons;
/*
  typedef std::map<FCUINT, DesktopOption> DesktopOptionMap;
	DesktopOptionMap						m_mapDesktopOptions;
  core::dimension2d<s32>			m_iconMax;
*/

  /*
   * Running applications on the desktop
   */
  typedef std::vector<InGameAppWindow*> AppWindowVector;
  AppWindowVector         		m_arrApps;
	PThreadMutex								m_mutexApps;							// Might want to change this to a read/write lock

  /*
   *  Arbitrary utility windows
   */
  SoftwareMgrWindow*          m_pSoftwareMgr;
};

#endif//_DESKTOP_H_