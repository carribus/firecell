#ifndef _GUICHATPANE_H_
#define _GUICHATPANE_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class GUIChatPane : public IGUIElement
{
public:
  GUIChatPane(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent = 0, SColor backColor = SColor(255, 255, 255, 255), SColor textColor = SColor(255, 0, 0, 255), s32 id = -1);
  ~GUIChatPane(void);

  void addLogItem(std::wstring str);

private:

  IGUIStaticText*     m_pChatLog;
  IGUIEditBox*        m_pChatEntry;
  SColor              m_backColor;
  SColor              m_textColor;
};

#endif//_GUICHATPANE_H_