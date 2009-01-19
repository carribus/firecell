#ifndef _DESKTOP_H_
#define _DESKTOP_H_

#include <vector>
#include <irrlicht.h>
#include "InGameAppWindow.h"

using namespace irr;
using namespace video;

class ViewLogicGame;

class Desktop
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
	};

  Desktop(ViewLogicGame& owner, IrrlichtDevice* pDevice);
  ~Desktop(void);

  bool Create();
  void Draw();
  bool GetDesktopOptionFromPt(s32 x, s32 y, DesktopOption* d);
  bool OpenApplication(FCULONG optionID, FCSHORT cpuCost, FCULONG memCost);
	bool IsApplicationRunning(FCUINT appType);

private:

  /*
   *  Private Methods
   */
  void UpdateDesktopOptions();
  void DrawDesktopIcons();

	InGameAppWindow* GetAppWindowByType(FCULONG type);

  /*
   *  Private Data Members
   */
  ViewLogicGame&              m_owner;
  IrrlichtDevice*             m_pDevice;
	ITexture*										m_pBackground;
	IGUIFont*										m_pFontCourier;

  /*
   *  Desktop option container and utility members
   */
  typedef std::map<FCUINT, DesktopOption> DesktopOptionMap;
	DesktopOptionMap						m_mapDesktopOptions;
  core::dimension2d<s32>			m_iconMax;

  /*
   * Running applications on the desktop
   */
  typedef std::vector<InGameAppWindow*> AppWindowVector;
  AppWindowVector         		m_arrApps;
	

};

#endif//_DESKTOP_H_