#ifndef _APPLETSYSMON_H_
#define _APPLETSYSMON_H_
#include "IAppBarApplet.h"

class DesktopAppBar;

class AppletSysMon : public IAppBarApplet
{
public:
  AppletSysMon(DesktopAppBar* pParent, IGUIEnvironment* pEnv);
  ~AppletSysMon(void);

  /*
   *  IAppBarApplet implementation
   */
  std::wstring getHoverText();
  void getAppletIconRect(core::rect<s32>& rect);
  void drawAppletIcon(core::rect<s32>& rect);
  void onAppletClicked();
  void OnAppletRightClicked();

private:

  IGUIEnvironment* m_pEnv;
  DesktopAppBar* m_pParent;
  ITexture* m_pIcon;

};

#endif//_APPLETSYSMON_H_