#include <sstream>
#include "../common/irrlichtUtil/irrfontfx.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "GUIForumThreadBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_HEADER_HEIGHT         30
#define FORUM_THREAD_ITEM_HEIGHT		45

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
	core::rect<s32> rect = AbsoluteRect;
  std::wstringstream ss;

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );
  drawHeader(pVideo);
//	drawColumnHeaders(pVideo
	
	// draw the threads
	const ForumThreadMap& threads = m_currentCategory->getThreads();
	ForumThreadMap::const_iterator it = threads.begin();
	ForumThreadMap::const_iterator limit = threads.end();

	rect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT*2;
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + FORUM_THREAD_ITEM_HEIGHT;

	for ( ; it != limit; it++ )
	{
		drawThread(it->second, rect);
		rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y;
		rect.LowerRightCorner.Y += FORUM_THREAD_ITEM_HEIGHT;
	}

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

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::drawThread(ForumThread* pThread, core::rect<s32> rect)
{
	std::wstringstream ss;
	core::rect<s32> cRect = rect;
	IVideoDriver* pVideo = Environment->getVideoDriver();
  IGUIFont* pFont = Environment->getSkin()->getFont();

	// draw the thread title
	ss << pThread->getTitle().c_str();
	cRect.UpperLeftCorner.X += 10;
	cRect.UpperLeftCorner.Y += 5;
	pFont->draw( ss.str().c_str(), cRect, m_textColor, false, false );
	ss.str(L"");

	// draw the author
	ss << L"Author: Blah blah";
	cRect.UpperLeftCorner.Y += 18;
	pFont->draw( ss.str().c_str(), cRect, SColor(128, 255, 255, 255), false, false );
	ss.str(L"");
	cRect.UpperLeftCorner.Y -= 23;

	// draw the date created
	cRect.UpperLeftCorner.X = cRect.LowerRightCorner.X - 200;
	pVideo->draw2DLine( cRect.UpperLeftCorner, position2d<s32>(cRect.UpperLeftCorner.X, cRect.LowerRightCorner.Y), m_textColor );
	cRect.UpperLeftCorner.X += 10;
	ss << pThread->getDateCreated().c_str();
	pFont->draw( ss.str().c_str(), cRect, m_textColor, false, true );

	// draw the seperator
	pVideo->draw2DLine( position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ),
											rect.LowerRightCorner );
}