#include <sstream>
#include "GUIChatPane.h"

GUIChatPane::GUIChatPane(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_backColor(backColor)
, m_textColor(textColor)
{
#ifdef _DEBUG
  setDebugName("GUIChatPane");
#endif//_DEBUG

  // create the chat log area
  core::rect<s32> r = rect;
  r.LowerRightCorner.Y -= 24;
  if ( (m_pChatLog = environment->addStaticText(L"", r, true, true, this, -1, true)) )
  {
    m_pChatLog->setBackgroundColor(backColor);
    m_pChatLog->setOverrideColor(textColor);
  }
 
}

GUIChatPane::~GUIChatPane(void)
{
}
