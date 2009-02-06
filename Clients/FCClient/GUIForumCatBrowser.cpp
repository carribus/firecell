#include "GUIForumCatBrowser.h"

GUIForumCatBrowser::GUIForumCatBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_backColor(backColor)
, m_textColor(textColor)
{
}

///////////////////////////////////////////////////////////////////////

GUIForumCatBrowser::~GUIForumCatBrowser(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::draw()
{
  IVideoDriver* pVideo = Environment->getVideoDriver();

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );
}

///////////////////////////////////////////////////////////////////////

bool GUIForumCatBrowser::OnEvent(const SEvent& event)
{
  bool bResult = false;

  return bResult;
}
