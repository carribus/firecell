#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include <vector>
#include <irrlicht.h>
#include "../../common/PThreadMutex.h"
#include "IDesktop.h"
#include "DesktopAppBar.h"
#include "DesktopIcon.h"
#include "InGameAppWindow.h"

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
  void GetDesktopRect(core::rect<s32>& rect);
/*
  bool GetDesktopOptionFromPt(s32 x, s32 y, DesktopOption* d);
	bool GetDesktopOptionByID(FCULONG optionID, DesktopOption& d);
	void HighlightDesktopOption(FCULONG optionID, bool bHighlight = true);
  void ClearAllHighlights();
*/
  bool OpenApplication(FCULONG optionID, FCSHORT cpuCost, FCULONG memCost);
	bool IsApplicationRunning(FCUINT appType);

	bool OnGUIEvent(SEvent::SGUIEvent event);
  bool OnConsoleEvent(FCModelEvent& event);

  /*
   *  IDesktop implementation
   */
	void OnDesktopIconSelected(DesktopIcon* pIcon);
  void OnApplicationActivated(InGameAppWindow* pApp);

private:

  /*
   *  Private Methods
   */
  void UpdateDesktopOptions();
  void updateIconPositions();
  void DrawDesktopIcons();

	InGameAppWindow* GetAppWindowByType(FCULONG type);

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
};

#endif//_DESKTOP_H_