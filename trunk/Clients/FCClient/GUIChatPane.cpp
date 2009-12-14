#include <sstream>
#include "GUIChatPane.h"

#define INPUT_AREA_HEIGHT 24

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
  r.LowerRightCorner.Y -= (24 + INPUT_AREA_HEIGHT);
  if ( (m_pChatLog = environment->addStaticText(L"", r, true, true, this, -1, true)) )
  {
    m_pChatLog->setBackgroundColor(backColor);
    m_pChatLog->setOverrideColor(textColor);
  }
  
  // create the input area
  r.UpperLeftCorner.Y = r.LowerRightCorner.Y + 2;
  r.LowerRightCorner.Y += INPUT_AREA_HEIGHT;
  if ( ( m_pChatEntry = environment->addEditBox(L"", r, true, this, -1) ) )
  {
    m_pChatEntry->setOverrideColor(textColor);
  }
 
}

///////////////////////////////////////////////////////////////////////

GUIChatPane::~GUIChatPane(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIChatPane::addLogItem(std::wstring str)
{
  std::wstring curLog = m_pChatLog->getText();

  curLog += str;
  m_pChatLog->setText(curLog.c_str());
}