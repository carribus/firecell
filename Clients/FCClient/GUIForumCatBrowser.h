#ifndef _GUIFORUMCATBROWSER_H_
#define _GUIFORUMCATBROWSER_H_

#include <string>
#include <vector>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class GUIForumCatBrowser : public IGUIElement
{
public:
  GUIForumCatBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 128, 128, 128), s32 id = -1);
  ~GUIForumCatBrowser(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

private:

  SColor            m_backColor;
  SColor            m_textColor;

};

#endif//_GUIFORUMCATBROWSER_H_