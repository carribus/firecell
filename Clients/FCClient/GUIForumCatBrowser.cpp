#include <sstream>
#include "irrSingleton.h"
#include "GUIForumCatBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_HORZ_INDENT_SIZE      20

#define FORUM_CATITEM_HEIGHT				30
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

GUIForumCatBrowser::~GUIForumCatBrowser(void)
{
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
  IVideoDriver* pVideo = Environment->getVideoDriver();

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );

	drawHeader(pVideo);

	rect<s32> cRect = AbsoluteClippingRect;
	CategoryVector::iterator it = m_categories.begin();
	CategoryVector::iterator limit = m_categories.end();

	// create the template rectangle
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
	ForumCatStruct fcs = { pCat, false, false };
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
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawForumCategory(ForumCatStruct& fcs, rect<s32>& rect, s32 level)
{
  IVideoDriver* pVideo = Environment->getVideoDriver();
	std::wstringstream ss;
	IGUIFont* pFont = Environment->getSkin()->getFont();

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

	// set the item rectangle
	fcs.rect = rect;
	// do a highlight if needed
	if ( fcs.bHighlighted )
		pVideo->draw2DRectangle( rect, SColor(255, 64, 64, 64), SColor(255, 64, 64, 64), SColor(255, 0, 0, 0), SColor(255, 0, 0, 0) );
/*
	// draw the rectangle border
	pVideo->draw2DLine( rect.UpperLeftCorner, position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y ), m_textColor );
	pVideo->draw2DLine( position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y), rect.LowerRightCorner, m_textColor );
	pVideo->draw2DLine( rect.LowerRightCorner, position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), m_textColor );
	pVideo->draw2DLine( position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), rect.UpperLeftCorner, m_textColor );
*/
	if ( fcs.pCat->getChildren().size() || fcs.pCat->getThreads().size() )
	{
		drawExpanderIcon(fcs, level, pVideo, pFont);
	}
	rect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	// draw the text
	ss << fcs.pCat->getName().c_str();
	pFont->draw( ss.str().c_str(), rect, m_textColor, false, true );
	ss.str(L"");
	rect.UpperLeftCorner.X -= FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + (level+1)*FORUM_HORZ_INDENT_SIZE;
	rect.UpperLeftCorner.Y += FORUM_CATITEM_HEIGHT;
	rect.LowerRightCorner.Y += FORUM_CATITEM_HEIGHT;

	// if the category is expanded, the draw the children
	if ( fcs.bExpanded )
	{
    // first, lets check if there are any threads in this category...
    const ForumThreadMap& threads = fcs.pCat->getThreads();
    if ( threads.size() )
      drawCategoryThreads(threads, rect);

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