#ifndef _DESKTOPAPPBAR_H_
#define _DESKTOPAPPBAR_H_

#include <irrlicht.h>
#include "InGameAppWindow.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class DesktopAppBar : public IGUIElement
{
public:
  DesktopAppBar(IGUIEnvironment* env, IGUIElement* pParent = 0, s32 id = -1);
  ~DesktopAppBar(void);

  void setActiveApp(InGameAppWindow* pApp);

  void draw();

  void setHeight(u32 height)                                { m_height = height; }
  u32 getHeight()                                           { return m_height; }

private:

  u32                 m_height;

  IGUIFont*           m_pClockFont;
  InGameAppWindow*    m_pActiveApp;
};

#endif//_DESKTOPAPPBAR_H_