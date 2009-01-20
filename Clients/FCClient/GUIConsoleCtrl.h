#ifndef _GUICONSOLECTRL_H_
#define _GUICONSOLECTRL_H_

#include <irrlicht.h>

namespace irr
{

using namespace video;

namespace gui
{

class GUIConsoleCtrl : public IGUIElement
{
public:
  GUIConsoleCtrl(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor textColor = SColor(255, 128, 128, 128), s32 id = -1);
  ~GUIConsoleCtrl(void);

  /*
   *  IGUIElement overrides
   */
  void draw();

  void setBackgroundColor(SColor col)                           { m_backColor = col; }
  void setTextColor(SColor col)                                 { m_textColor = col; }
  SColor getBackgroundColor()                                   { return m_backColor; }
  SColor getTextColor()                                         { return m_textColor; }

protected:

  SColor            m_backColor;
  SColor            m_textColor;
};

} // end of namespace gui

} // end of namespace irr

#endif// _GUICONSOLECTRL_H_