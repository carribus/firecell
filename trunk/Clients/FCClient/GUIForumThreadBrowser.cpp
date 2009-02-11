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
#ifdef _DEBUG
  setDebugName("GUIForumThreadBrowser");
#endif//_DEBUG
	core::rect<s32> sbRect = rect;

	sbRect.UpperLeftCorner.Y = 0;
	sbRect.LowerRightCorner.X -= 2;
	sbRect.UpperLeftCorner.X = sbRect.LowerRightCorner.X - FORUM_SCROLLBAR_WIDTH;
	sbRect.LowerRightCorner.Y = sbRect.UpperLeftCorner.Y + rect.getHeight();
	IGUIScrollBar* pSB = environment->addScrollBar(false, sbRect, this);
	pSB->setMax(0);
	pSB->setSubElement(true);
	pSB->setVisible(true);

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;
}

///////////////////////////////////////////////////////////////////////

GUIForumThreadBrowser::~GUIForumThreadBrowser(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::updateCategoryThreads(FCULONG category_id)
{
  if ( !m_pModel )
    return;
  if ( !m_currentCategory )
    return;
  if ( m_currentCategory->getID() != category_id )
    return;

  const ForumThreadMap& threads = m_currentCategory->getThreads();
  ForumThreadMap::const_iterator it = threads.begin();
  ForumThreadMap::const_iterator limit = threads.end();

  for ( ; it != limit; it++ )
  {
    addThread( it->second );
  }
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
//	drawColumnHeaders(pVideo
	
  AbsoluteClippingRect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT;

	// draw the threads
	ThreadVector::const_iterator it = m_threads.begin();
	ThreadVector::const_iterator limit = m_threads.end();

	for ( ; it != limit; it++ )
	{
		drawThread(*it);
	}

  AbsoluteClippingRect.UpperLeftCorner.Y -= FORUM_HEADER_HEIGHT;

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

void GUIForumThreadBrowser::drawThread(const ForumThreadStruct& thread)
{
	std::wstringstream ss;
	IVideoDriver* pVideo = Environment->getVideoDriver();
  IGUIFont* pFont = Environment->getSkin()->getFont();
  core::rect<s32> cRect = thread.rect;

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

  // draw the thread title
	ss << thread.pThread->getTitle().c_str();
	cRect.UpperLeftCorner.X += 10;
	cRect.UpperLeftCorner.Y += 5;
	pFont->draw( ss.str().c_str(), cRect, m_textColor, false, false, &AbsoluteClippingRect );
	ss.str(L"");

	// draw the author
	ss << L"Author: Blah blah";
	cRect.UpperLeftCorner.Y += 18;
	pFont->draw( ss.str().c_str(), cRect, SColor(128, 255, 255, 255), false, false, &AbsoluteClippingRect );
	ss.str(L"");
	cRect.UpperLeftCorner.Y -= 23;

	// draw the date created
	cRect.UpperLeftCorner.X = cRect.LowerRightCorner.X - 200;
  if ( cRect.UpperLeftCorner.Y > AbsoluteClippingRect.UpperLeftCorner.Y &&
       cRect.UpperLeftCorner.Y < AbsoluteClippingRect.LowerRightCorner.Y )
  {
	  pVideo->draw2DLine( cRect.UpperLeftCorner, position2d<s32>(cRect.UpperLeftCorner.X, cRect.LowerRightCorner.Y), m_textColor );
  }
	cRect.UpperLeftCorner.X += 10;
	ss << thread.pThread->getDateCreated().c_str();
	pFont->draw( ss.str().c_str(), cRect, m_textColor, false, true, &AbsoluteClippingRect );

	// draw the seperator
  if ( cRect.UpperLeftCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
    cRect.UpperLeftCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
  if ( cRect.LowerRightCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
    cRect.LowerRightCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
  if ( cRect.LowerRightCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
    cRect.LowerRightCorner.Y = AbsoluteClippingRect.LowerRightCorner.Y;
	pVideo->draw2DLine( position2d<s32>( AbsoluteClippingRect.UpperLeftCorner.X, cRect.LowerRightCorner.Y ),
											cRect.LowerRightCorner );
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::addThread(ForumThread* pThread)
{
  if ( !pThread )
    return;

  ForumThreadStruct fts = { pThread, false };

  if ( m_threads.size() > 0 )
  {
    ForumThreadStruct& prev = m_threads[ m_threads.size()-1 ];
    fts.rect.UpperLeftCorner.X = AbsoluteClippingRect.UpperLeftCorner.X;
    fts.rect.UpperLeftCorner.Y = prev.rect.LowerRightCorner.Y;
    fts.rect.LowerRightCorner.X = AbsoluteClippingRect.LowerRightCorner.X;
    fts.rect.LowerRightCorner.Y = fts.rect.UpperLeftCorner.Y + FORUM_THREAD_ITEM_HEIGHT;
  }
  else
  {
    fts.rect.UpperLeftCorner = position2d<s32>(AbsoluteClippingRect.UpperLeftCorner.X, AbsoluteClippingRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT);
    fts.rect.LowerRightCorner.Y = fts.rect.UpperLeftCorner.Y + FORUM_THREAD_ITEM_HEIGHT;
    fts.rect.LowerRightCorner.X = AbsoluteClippingRect.LowerRightCorner.X;
  }

	m_threads.push_back(fts);
}