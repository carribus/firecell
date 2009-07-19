#ifndef _APPLETSYSMON_H_
#define _APPLETSYSMON_H_

#include "AppBarAppletBase.h"

class AppletSysMon : public AppBarAppletBase
{
public:
  AppletSysMon(DesktopAppBar* pParent, IGUIEnvironment* pEnv);
  ~AppletSysMon(void);

  std::wstring getHoverText();
  void onAppletClicked();
  void OnAppletRightClicked();

protected:

  void onAppletHover(bool bHover);

private:


};

#endif//_APPLETSYSMON_H_