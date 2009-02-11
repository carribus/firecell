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
#ifdef _DEBUG
  setDebugName("GUIForumCatBrowser");
#endif//_DEBUG
	core::rect<s32> sbRect = rect;

	sbRect.UpperLeftCorner.Y = FORUM_HEADER_HEIGHT;
	sbRect.LowerRightCorner.X -= 2;
	sbRect.UpperLeftCorner.X = sbRect.LowerRightCorner.X - FORUM_SCROLLBAR_WIDTH;
	sbRect.LowerRightCorner.Y = rect.getHeight();

	m_pSB = environment->addScrollBar(false, sbRect, this);
	m_pSB->setMax(0);
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

void GUIForumCatBrowser::draw()
{
  if ( !IsVisible )
    return;

  IVideoDriver* pVideo = Environment->getVideoDriver();

  m_bOverflow = false;
  m_pixelOverflow = 0;
  m_pixelUnderflow = (u32)m_pSB->getPos();

  // draw the background
  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );
  // draw the Category Browser header
	drawHeader(pVideo);

  AbsoluteClippingRect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT;

	CategoryVector::iterator it = m_categories.begin();
	CategoryVector::iterator limit = m_categories.end();

	for ( ; it != limit; it++ )
	{
		if ( it->pCat->getParentID() == 0 )
    {
			drawForumCategory(*it);
    }
	}

  // if there is overflow, update the scrollbar
  if ( m_bOverflow )
  {
    m_pSB->setMax( m_pixelOverflow + m_pixelUnderflow );
  }

  AbsoluteClippingRect.UpperLeftCorner.Y -= FORUM_HEADER_HEIGHT;

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
          m_mousePos.X = event.MouseInput.X;
          m_mousePos.Y = event.MouseInput.Y;
          checkHighlights();
				}
				break;

			case	EMIE_LMOUSE_PRESSED_DOWN:
				{
					bResult = OnLButtonDown(event.MouseInput);
				}
				break;

      case  EMIE_MOUSE_WHEEL:
        {
          Environment->setFocus(m_pSB);
          bResult = m_pSB->OnEvent(event);
        }

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

      case  EGET_SCROLL_BAR_CHANGED:
        {
          s32 diff = m_pSB->getPos()-m_pixelUnderflow;
          if ( diff )
          {
            CategoryVector::iterator it = m_categories.begin();
            CategoryVector::iterator limit = m_categories.end();

            for ( ; it != limit; it++ )
            {
              offsetRect( it->rect, 0, -diff );
            }
          }
          m_pixelUnderflow = m_pSB->getPos();
          checkHighlights();
          bResult = true;
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
  if ( !pCat )
    return;

	ForumCatStruct fcs = { pCat, true, false };

  if ( m_categories.size() > 0 )
  {
    ForumCatStruct& prev = m_categories[ m_categories.size()-1 ];
    fcs.rect.UpperLeftCorner.X = AbsoluteClippingRect.UpperLeftCorner.X;
    fcs.rect.UpperLeftCorner.Y = prev.rect.LowerRightCorner.Y;
    fcs.rect.LowerRightCorner.X = AbsoluteClippingRect.LowerRightCorner.X;
    fcs.rect.LowerRightCorner.Y = fcs.rect.UpperLeftCorner.Y + FORUM_CATITEM_HEIGHT;
  }
  else
  {
    fcs.rect.UpperLeftCorner = position2d<s32>(AbsoluteClippingRect.UpperLeftCorner.X, AbsoluteClippingRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT);
    fcs.rect.LowerRightCorner.Y = fcs.rect.UpperLeftCorner.Y + FORUM_CATITEM_HEIGHT;
    fcs.rect.LowerRightCorner.X = AbsoluteClippingRect.LowerRightCorner.X;
  }

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

void GUIForumCatBrowser::drawForumCategory(ForumCatStruct& fcs, s32 level)
{
  IVideoDriver* pVideo = Environment->getVideoDriver();
	std::wstringstream ss;
  core::rect<s32> rect;
	IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkGrey(255, 64, 64, 64), black(255, 0, 0, 0);
  u32 overFlow = 0;

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

  if ( (overFlow = checkForOverflow(fcs.rect)) > 0 )
  {
    // add to the existing pixel overflow
    m_pixelOverflow += overFlow;
  }

  if ( fcs.rect.UpperLeftCorner.Y < AbsoluteClippingRect.LowerRightCorner.Y )
  {
	  // do a highlight if needed
	  if ( fcs.bHighlighted )
		  pVideo->draw2DRectangle( fcs.rect, darkGrey, darkGrey, black, black, &AbsoluteClippingRect );
	  // draw lines across the top and bottom of the rectangle
    if ( fcs.rect.UpperLeftCorner.Y >= AbsoluteClippingRect.UpperLeftCorner.Y )
	    pVideo->draw2DLine( position2d<s32>(AbsoluteRect.UpperLeftCorner.X, fcs.rect.UpperLeftCorner.Y), position2d<s32>( fcs.rect.LowerRightCorner.X, fcs.rect.UpperLeftCorner.Y ), m_textColor );
    if ( fcs.rect.LowerRightCorner.Y <= AbsoluteClippingRect.LowerRightCorner.Y && fcs.rect.LowerRightCorner.Y >= AbsoluteClippingRect.UpperLeftCorner.Y )
	    pVideo->draw2DLine( fcs.rect.LowerRightCorner, position2d<s32>( AbsoluteRect.UpperLeftCorner.X, fcs.rect.LowerRightCorner.Y ), m_textColor );

    // draw the expander/collapser icon if needed
    if ( fcs.pCat->getChildren().size() )
	  {
		  drawExpanderIcon(fcs, level, pVideo, pFont);
	  }

    // pad the item
    rect = fcs.rect;
		rect.UpperLeftCorner.X += level*FORUM_HORZ_INDENT_SIZE;
	  rect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT + FORUM_EXPANDER_ICON_WIDTH;
	  // draw the text
    rect.UpperLeftCorner.Y += 10;
	  ss << fcs.pCat->getName().c_str();
	  pFont->draw( ss.str().c_str(), rect, m_textColor, false, false, &AbsoluteClippingRect );
	  ss.str(L"");
    // draw the description
    rect.UpperLeftCorner.Y += 20;
    ss << fcs.pCat->getDesc().c_str();
    m_pFontDesc->draw( ss.str().c_str(), rect, SColor(255, 164, 164, 164), false, false, &AbsoluteClippingRect );
    ss.str(L"");
    rect.UpperLeftCorner.Y -= 30;

    // draw the thread count
    core::rect<s32> cntRect = core::rect<s32>( position2d<s32>( fcs.rect.LowerRightCorner.X-FORUM_CATVIEW_THREADCOUNT_WIDTH, fcs.rect.UpperLeftCorner.Y ),
                                               position2d<s32>( fcs.rect.LowerRightCorner.X, fcs.rect.LowerRightCorner.Y ));

    if ( cntRect.UpperLeftCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
      cntRect.UpperLeftCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
    if ( cntRect.LowerRightCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
      cntRect.LowerRightCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
    if ( cntRect.LowerRightCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
      cntRect.LowerRightCorner.Y = AbsoluteClippingRect.LowerRightCorner.Y;
    pVideo->draw2DLine( cntRect.UpperLeftCorner, position2d<s32>( cntRect.UpperLeftCorner.X, cntRect.LowerRightCorner.Y ), m_textColor );

    cntRect = core::rect<s32>( position2d<s32>( fcs.rect.LowerRightCorner.X-FORUM_CATVIEW_THREADCOUNT_WIDTH, fcs.rect.UpperLeftCorner.Y ),
                               position2d<s32>( fcs.rect.LowerRightCorner.X, fcs.rect.LowerRightCorner.Y ));
    ss << fcs.pCat->getThreadCount();
    pFont->draw( ss.str().c_str(), cntRect, m_textColor, true, true, &AbsoluteClippingRect );

	  // if the category is expanded, then draw the children
	  if ( fcs.bExpanded )
	  {
		  // next, try and draw the child categories
		  std::vector<ForumCatStruct>::iterator it = m_categories.begin();
		  std::vector<ForumCatStruct>::iterator limit = m_categories.end();

		  for ( ; it != limit; it++ )
		  {
			  if ( it->pCat->getParentID() == fcs.pCat->getID() )
			    drawForumCategory(*it, level+1);
		  }
	  }
    rect.UpperLeftCorner.X = AbsoluteRect.UpperLeftCorner.X + (level)*FORUM_HORZ_INDENT_SIZE;
  }
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::drawExpanderIcon(ForumCatStruct& fcs, s32 level, IVideoDriver* pVideo, IGUIFont* pFont)
{
	if ( !pVideo || !pFont )
		return;

	core::rect<s32> eRect = fcs.rect;

	eRect.UpperLeftCorner.X += FORUM_CATITEM_PADDING_LEFT;
	pFont->draw( fcs.bExpanded ? L"-" : L"+", eRect, m_textColor, false, true, &AbsoluteClippingRect );
}

///////////////////////////////////////////////////////////////////////

u32 GUIForumCatBrowser::checkForOverflow(core::rect<s32> rect)
{
  u32 pixelOverflow = 0;

  if ( rect.LowerRightCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
  {
    m_bOverflow = true;
    pixelOverflow = rect.LowerRightCorner.Y - AbsoluteClippingRect.LowerRightCorner.Y;
    if ( pixelOverflow > (u32)rect.getHeight() )
      pixelOverflow = rect.getHeight();
  }

  return pixelOverflow;
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::checkHighlights()
{
	// do the highlighting
	std::vector<ForumCatStruct>::iterator it = m_categories.begin();
	std::vector<ForumCatStruct>::iterator limit = m_categories.end();

  AbsoluteClippingRect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT;
	for ( ; it != limit; it++ )
	{
		if ( AbsoluteClippingRect.isPointInside( m_mousePos ) &&
          it->rect.isPointInside( m_mousePos ) )
		{
			it->bHighlighted = true;
		}
		else
		{
			it->bHighlighted = false;
		}
	}
  AbsoluteClippingRect.UpperLeftCorner.Y -= FORUM_HEADER_HEIGHT;
}

///////////////////////////////////////////////////////////////////////

void GUIForumCatBrowser::offsetRect(core::rect<s32>& rect, s32 xOffs, s32 yOffs)
{
  rect.UpperLeftCorner.Y += yOffs;
  rect.UpperLeftCorner.X += xOffs;
  rect.LowerRightCorner.Y += yOffs;
  rect.LowerRightCorner.X += xOffs;
}