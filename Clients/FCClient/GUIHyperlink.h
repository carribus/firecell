#ifndef _GUIHYPERLINK_H_
#define _GUIHYPERLINK_H_

#include <irrlicht.h>

#define EGET_HYPERLINK_CLICKED      EGET_TABLE_SELECTED_AGAIN+1

namespace irr
{

using namespace video;

namespace gui
{

class GUIHyperlink : public IGUIElement
{
public:
  GUIHyperlink(IGUIEnvironment* env, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(0, 0, 0, 0), SColor textColor = SColor(255, 0, 0, 255), s32 id = -1);
  ~GUIHyperlink(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

  /*
   *  PUBLIC Methods
   */
  void setBackColor(SColor col)                       { m_backColor = col; }
  void setTextColor(SColor col)                       { m_textColor = col; }

  void onHyperlinkClicked();

private:

  IGUIEnvironment*          m_pEnv;
  SColor                    m_backColor;
  SColor                    m_textColor;
};


} // end of namespace gui

} // end of namespace irr

#endif//_GUIHYPERLINK_H_