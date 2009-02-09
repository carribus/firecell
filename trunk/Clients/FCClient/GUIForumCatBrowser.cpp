#include <sstream>
#include "clientstrings.h"
#include "../common/ResourceManager.h"
#include "irrSingleton.h"
#include "GUIForumCatBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_HORZ_INDENT_SIZE      20

#define FORUM_HEADER_HEIGHT         30
#define FORUM_CATVIEW_THREADCOUNT_WIDTH   125

#define FORUM_CATITEM_HEIGHT				50
#define FORUM_CATITEM_PADDING_LEFT	15

#define FORUM_THREADITEM_HEIGHT     25

#define FORUM_EXPANDER_ICON_WIDTH		16
#define FORUM_EXPANDER_ICON_HEIGHT	16

GUIForumCatBrowser::GUIForumCatBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
, m_pSink(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_currentCategory(NULL)
, m_pFontDesc(NULL)
, m_pSB(NULL)
{
	core::rect<s32> sbRect = rect;

	sbRect.UpperLeftCorner.Y = FORUM_HEADER_HEIGHT;
	sbRect.LowerRightCorner.X -= 2;
	sbRect.UpperLeftCorner.X = sbRect.LowerRightCorner.X - FORUM_SCROLLBAR_WIDTH;
	sbRect.LowerRightCorner.Y = rect.getHeight();

	m_pSB = environment->addScrollBar(false, sbRect, this);
	m_pSB->setMax(0);
	m_pSB->setPos(0);
	m_pSB->setSubElement(true);
	m_pSB->setVisible(true);

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;

  // create the description font
  m_pFontDesc = Environment->getFont("./clientdata/fonts/fontlucidaconsole_8px.xml");
}

///////////////////////////////////////////////////////////////////////

GUIForumCatBrowser::~GUIForumCatBrowser(void)
{
  if ( m_pFontDesc )
    m_pFontDesc->drop();
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::updateCategories()
{
	if ( !m_pModel )
		return;

	ForumCategory* pCat = m_pModel->getRootCategory();
	const ForumCategoryMap& children = pCat->getChildren();
	ForumCategoryMap::const_iterator it = children.begin();
	ForumCategoryMap::const_iterator limit = children.end();

	for ( ; it != limit; it++ )
	{
		addCategory( it->second );
	}
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::updateCategoryThreads(FCULONG category_id)
{
  if ( !m_pModel )
    return;
/*
  ForumCategory* pCat = m_pModel->getCategoryByID(category_id);

  if ( pCat )
  {
    const ForumThreadMap& threads = pCat->getThreads();
    ForumThreadMap::const_iterator it = threads.begin();
    ForumThreadMap::const_iterator limit = threads.end();

    for ( ; it != limit; it++ )
    {

    }
  }
*/
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::draw()
{
  if ( !IsVisible )
    return;

  IVideoDriver* pVideo = Environment->getVideoDriver();

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );

	drawHeader(pVideo);

	rect<s32> cRect = AbsoluteClippingRect;
	CategoryVector::iterator it = m_categories.begin();
	CategoryVector::iterator limit = m_categories.end();

	// create the template rectangle
  cRect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT;
	cRect.LowerRightCorner.Y = cRect.UpperLeftCorner.Y + FORUM_CATITEM_HEIGHT;

	for ( ; it != limit; it++ )
	{
		if ( it->pCat->getParentID() == 0 )
			drawForumCategory(*it, cRect);
	}

	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool GUIForumCatBrowser::OnEvent(const SEvent& event)
{
  bool bResult = false;

	switch ( event.EventType )
	{
	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_MOUSE_MOVED:
				{
					// do the highlighting
					std::vector<ForumCatStruct>::iterator it = m_categories.begin();
					std::vector<ForumCatStruct>::iterator limit = m_categories.end();

					for ( ; it != limit; it++ )
					{
						if ( it->rect.isPointInside( position2d<s32>( event.MouseInput.X, event.MouseInput.Y ) ) )
						{
							it->bHighlighted = true;
						}
						else
						{
							it->bHighlighted = false;
						}
					}
				}
				break;

			case	EMIE_LMOUSE_PRESSED_DOWN:
				{
					bResult = OnLButtonDown(event.MouseInput);
				}
				break;

			default:
				break;
			}
		}
		break;

	case	EET_GUI_EVENT:
		{
			switch ( event.GUIEvent.EventType )
			{
			case	EGET_ELEMENT_FOCUSED:
        {
          static bool bIgnoreFocus = false;
          if ( Parent )
          {
            if ( !bIgnoreFocus )              // avoid re-entry
            {
              Environment->setFocus(Parent);
              bIgnoreFocus = true;
              Environment->setFocus(this);
              bIgnoreFocus = false;
            }
          }
        }
				break;

			default:
				break;
			}
		}
		break;

	default:
		break;
	}

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool GUIForumCatBrowser::OnLButtonDown(const SEvent::SMouseInput& event)
{
  ITimer* pTimer = irrSingleton::instance().getTimer();
	u32 now = pTimer->getTime();

	if ( m_LButtonLastClick.last_tick )
	{
		if ( (event.X > m_LButtonLastClick.lastX-3 && event.X < m_LButtonLastClick.lastX+3) &&
			   (event.Y > m_LButtonLastClick.lastY-3 && event.Y < m_LButtonLastClick.lastY+3) )
		{
			if ( now - m_LButtonLastClick.last_tick <= 300 )
			{
				memset( &m_LButtonLastClick, 0, sizeof(LastClick) );
				return OnLButtonDblClick(event);
			}
		}
	}

	// update the tracking structure
	m_LButtonLastClick.last_tick = now;
	m_LButtonLastClick.lastX = event.X;
	m_LButtonLastClick.lastY = event.Y;
/*
	// expand/collapse (if possible) the clicked on category
	std::vector<ForumCatStruct>::iterator it = m_categories.begin();
	std::vector<ForumCatStruct>::iterator limit = m_categories.end();

	for ( ; it != limit; it++ )
	{
		if ( it->rect.isPointInside( position2d<s32>( event.X, event.Y ) ) )
		{
			it->bExpanded ^= true;
		}
	}
*/

  return true;
}

///////////////////////////////////////////////////////////////////////

bool GUIForumCatBrowser::OnLButtonDblClick(const SEvent::SMouseInput& event)
{
	std::vector<ForumCatStruct>::iterator it = m_categories.begin();
	std::vector<ForumCatStruct>::iterator limit = m_categories.end();

	for ( ; it != limit; it++ )
	{
		if ( it->rect.isPointInside( position2d<s32>( event.X, event.Y ) ) )
		{
			if ( m_pSink )
				m_pSink->OnCategorySelected( it->pCat->getID() );
			break;
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::addCategory(ForumCategory* pCat)
{
	ForumCatStruct fcs = { pCat, true, false };
	m_categories.push_back(fcs);

	const ForumCategoryMap& children = pCat->getChildren();
	ForumCategoryMap::const_iterator it = children.begin();
	ForumCategoryMap::const_iterator limit = children.end();

	for ( ; it != limit; it++ )
	{
		addCategory( it->second);
	}
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawHeader(IVideoDriver* pVideo)
{
  IGUIFont* pFont = Environment->getSkin()->getFont();
	rect<s32> hRect = AbsoluteClippingRect;
  SColor darkRed(128, 255, 0, 0), black(128, 0, 0, 0), white(255, 255, 255, 255);
  hRect.LowerRightCorner.Y = hRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT;
	hRect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X;

  pVideo->draw2DRectangle( hRect, darkRed, darkRed, black, black );
  hRect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT;

  // draw the first title
  pFont->draw( ResourceManager::instance().GetClientString( STR_APP_FORUM_HEADER_FORUMCAT ).c_str(), hRect, white, false, true );

  // draw the thread count title
  hRect.UpperLeftCorner.X = hRect.LowerRightCorner.X - FORUM_CATVIEW_THREADCOUNT_WIDTH;
  pFont->draw( ResourceManager::instance().GetClientString( STR_APP_FORUM_HEADER_THREADCOUNT ).c_str(), hRect, white, true, true );
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawForumCategory(ForumCatStruct& fcs, rect<s32>& rect, s32 level)
{
  IVideoDriver* pVideo = Environment->getVideoDriver();
	std::wstringstream ss;
	IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkGrey(255, 64, 64, 64), black(255, 0, 0, 0);

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

	// set the item rectangle
	fcs.rect = rect;
	fcs.rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X;
	// do a highlight if needed
	if ( fcs.bHighlighted )
		pVideo->draw2DRectangle( fcs.rect, darkGrey, darkGrey, black, black );
	// draw lines across the top and bottom of the rectangle
	pVideo->draw2DLine( position2d<s32>(AbsoluteRect.UpperLeftCorner.X, rect.UpperLeftCorner.Y), position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y ), m_textColor );
	pVideo->draw2DLine( rect.LowerRightCorner, position2d<s32>( AbsoluteRect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), m_textColor );

  // draw the expander/collapser icon if needed
  if ( fcs.pCat->getChildren().size() )
	{
		drawExpanderIcon(fcs, level, pVideo, pFont);
	}

  // pad the item
	rect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	// draw the text
  rect.UpperLeftCorner.Y += 10;
	ss << fcs.pCat->getName().c_str();
	pFont->draw( ss.str().c_str(), rect, m_textColor, false, false );
	ss.str(L"");
  // draw the description
  rect.UpperLeftCorner.Y += 20;
  ss << fcs.pCat->getDesc().c_str();
  m_pFontDesc->draw( ss.str().c_str(), rect, SColor(255, 164, 164, 164), false, false );
  ss.str(L"");
  rect.UpperLeftCorner.Y -= 30;

  // draw the thread count
  core::rect<s32> cntRect( position2d<s32>( rect.LowerRightCorner.X-FORUM_CATVIEW_THREADCOUNT_WIDTH, rect.UpperLeftCorner.Y ),
                     position2d<s32>( rect.LowerRightCorner.X, rect.LowerRightCorner.Y ));

  pVideo->draw2DLine( cntRect.UpperLeftCorner, position2d<s32>( cntRect.UpperLeftCorner.X, cntRect.LowerRightCorner.Y ), m_textColor );
  ss << fcs.pCat->getThreadCount();
  pFont->draw( ss.str().c_str(), cntRect, m_textColor, true, true );

	// offset the rectangle downwards
	rect.UpperLeftCorner.X -= FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + (level+1)*FORUM_HORZ_INDENT_SIZE;
	rect.UpperLeftCorner.Y += FORUM_CATITEM_HEIGHT;
	rect.LowerRightCorner.Y += FORUM_CATITEM_HEIGHT;

	// if the category is expanded, the draw the children
	if ( fcs.bExpanded )
	{
/*
		// first, lets check if there are any threads in this category...
		const ForumThreadMap& threads = fcs.pCat->getThreads();
		if ( threads.size() )
			drawCategoryThreads(threads, rect);
*/
		// next, try and draw the child categories
		std::vector<ForumCatStruct>::iterator it = m_categories.begin();
		std::vector<ForumCatStruct>::iterator limit = m_categories.end();

		for ( ; it != limit; it++ )
		{
			if ( it->pCat->getParentID() == fcs.pCat->getID() )
				drawForumCategory(*it, rect, level+1);
		}
	}

	rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + (level)*FORUM_HORZ_INDENT_SIZE;
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawExpanderIcon(ForumCatStruct& fcs, s32 level, IVideoDriver* pVideo, IGUIFont* pFont)
{
	if ( !pVideo || !pFont )
		return;

	core::rect<s32> eRect = fcs.rect;

	eRect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT;
	pFont->draw( fcs.bExpanded ? L"-" : L"+", eRect, m_textColor, false, true );
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawCategoryThreads(const ForumThreadMap& threads, rect<s32>& rect)
{
  IVideoDriver* pVideo = Environment->getVideoDriver();
	std::wstringstream ss;
	IGUIFont* pFont = Environment->getSkin()->getFont();

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + FORUM_THREADITEM_HEIGHT;

  ForumThreadMap::const_iterator it = threads.begin();
  ForumThreadMap::const_iterator limit = threads.end();

  for ( ; it != limit; it++ )
  {
	  // draw the rectangle border
	  pVideo->draw2DLine( rect.UpperLeftCorner, position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y ), m_textColor );
	  pVideo->draw2DLine( position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y), rect.LowerRightCorner, m_textColor );
	  pVideo->draw2DLine( rect.LowerRightCorner, position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), m_textColor );
	  pVideo->draw2DLine( position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), rect.UpperLeftCorner, m_textColor );

	  // draw the text
	  ss << it->second->getTitle().c_str();
	  pFont->draw( ss.str().c_str(), rect, m_textColor, false, true );
	  ss.str(L"");

    // move the rect down one item
    rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y;
    rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + FORUM_THREADITEM_HEIGHT;
  }

	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + FORUM_CATITEM_HEIGHT;
}