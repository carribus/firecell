#ifndef _APPLETSYSMON_H_
#define _APPLETSYSMON_H_
#include "IAppBarApplet.h"

class AppletSysMon : public IAppBarApplet
{
public:
  AppletSysMon(DesktopAppBar* pParent);
  ~AppletSysMon(void);

  /*
   *  IAppBarApplet implementation
   */
  std::wstring getHoverText();
  void drawAppletIcon(core::rect<s32>& rect);
  void onAppletClicked();
  void OnAppletRightClicked();

};

#endif//_APPLETSYSMON_H_