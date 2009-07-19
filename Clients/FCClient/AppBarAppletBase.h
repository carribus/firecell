#ifndef _APPBARAPPLETBASE_H_
#define _APPBARAPPLETBASE_H_

#include "IAppBarApplet.h"

class DesktopAppBar;

class AppBarAppletBase : public IGUIElement
                       , public IAppBarApplet
{
public:
  AppBarAppletBase(DesktopAppBar* pParent, IGUIEnvironment* pEnv, const char* iconFilename);
  virtual ~AppBarAppletBase(void);

  /*
   *  IAppBarApplet implementation
   */
  void getAppletIconRect(core::rect<s32>& rect);
  void drawAppletIcon(core::rect<s32>& rect);

  bool OnEvent(const SEvent& event);
  void draw();

protected:

  virtual void onAppletHover(bool bHover) {};

  IGUIEnvironment* m_pEnv;
  DesktopAppBar* m_pParent;
  ITexture* m_pIcon;
};

#endif//_APPBARAPPLETBASE_H_