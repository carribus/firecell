#include <sstream>
#include "../common/irrlichtUtil/irrfontfx.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "GUIForumThreadBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_HEADER_HEIGHT         30

GUIForumThreadBrowser::GUIForumThreadBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
, m_pSink(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_currentCategory(NULL)
{
	core::rect<s32> sbRect = rect;

	sbRect.UpperLeftCorner.Y = 0;
	sbRect.LowerRightCorner.X -= 2;
	sbRect.UpperLeftCorner.X = sbRect.LowerRightCorner.X - FORUM_SCROLLBAR_WIDTH;
	sbRect.LowerRightCorner.Y = sbRect.UpperLeftCorner.Y + rect.getHeight();
	IGUIScrollBar* pSB = environment->addScrollBar(false, sbRect, this);
	pSB->setMax(0);
	pSB->setPos(0);
	pSB->setSubElement(true);
	pSB->setVisible(true);

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;
}

///////////////////////////////////////////////////////////////////////

GUIForumThreadBrowser::~GUIForumThreadBrowser(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::draw()
{
  if ( !IsVisible )
    return;

  IVideoDriver* pVideo = Environment->getVideoDriver();
  IGUIFont* pFont = Environment->getSkin()->getFont();
  std::wstringstream ss;

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );

  drawHeader(pVideo);

  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::drawHeader(IVideoDriver* pVideo)
{
  std::wstringstream ss;
  core::rect<s32> hRect = AbsoluteClippingRect;
  IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkBlue(128, 0, 0, 255), black(255, 0, 0, 0), whiteAlpha(128, 255, 255, 255), white(255, 255, 255, 255);

  hRect.LowerRightCorner.Y = hRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT;
  pVideo->draw2DRectangle( hRect, darkBlue, darkBlue, whiteAlpha, whiteAlpha, &AbsoluteClippingRect );
  // draw the header
  hRect.UpperLeftCorner.X += 10;
  ss << ResourceManager::instance().GetClientString( STR_APP_FORUM_CATEGORY ).c_str() << L": " << m_currentCategory->getName().c_str();
  drawStrokedFont( pFont, ss.str().c_str(), black, white, hRect, false, true );
}