#include <sstream>
#include "irrSingleton.h"
#include "GUIForumCatBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_CATITEM_HEIGHT				30
#define FORUM_CATITEM_PADDING_LEFT	15
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
	// draw the rectangle border
	pVideo->draw2DLine( rect.UpperLeftCorner, position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y ), m_textColor );
	pVideo->draw2DLine( position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y), rect.LowerRightCorner, m_textColor );
	pVideo->draw2DLine( rect.LowerRightCorner, position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), m_textColor );
	pVideo->draw2DLine( position2d<s32>( rect.UpperLeftCorner.X, rect.LowerRightCorner.Y ), rect.UpperLeftCorner, m_textColor );
	if ( fcs.pCat->getChildren().size() )
	{
		drawExpanderIcon(fcs, level, pVideo, pFont);
	}
	rect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	// draw the text
	ss << fcs.pCat->getName().c_str();
	pFont->draw( ss.str().c_str(), rect, m_textColor, false, true );
	ss.str(L"");
	rect.UpperLeftCorner.X -= FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	rect.UpperLeftCorner.X += (level+1)*25;
	rect.UpperLeftCorner.Y += FORUM_CATITEM_HEIGHT;
	rect.LowerRightCorner.Y += FORUM_CATITEM_HEIGHT;

	// if the category is expanded, the draw the children
	if ( fcs.bExpanded )
	{
		std::vector<ForumCatStruct>::iterator it = m_categories.begin();
		std::vector<ForumCatStruct>::iterator limit = m_categories.end();

		for ( ; it != limit; it++ )
		{
			if ( it->pCat->getParentID() == fcs.pCat->getID() )
				drawForumCategory(*it, rect, level+1);
		}
	}

	rect.UpperLeftCorner.X -= (level+1)*25;
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