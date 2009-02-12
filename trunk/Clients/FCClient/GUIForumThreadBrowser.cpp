#include <sstream>
#include "../common/irrlichtUtil/irrfontfx.h"
#include "../common/irrlichtUtil/irrutils.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "DlgNewThread.h"
#include "irrSingleton.h"
#include "GUIForumThreadBrowser.h"

#define FORUM_SCROLLBAR_WIDTH				20
#define FORUM_OPTIONBAR_HEIGHT			30
#define FORUM_HEADER_HEIGHT         30
#define FORUM_THREAD_ITEM_HEIGHT		45

GUIForumThreadBrowser::GUIForumThreadBrowser(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
, m_pSink(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_currentCategory(NULL)
, m_bOverflow(false)
, m_pixelOverflow(0)
, m_pixelUnderflow(0)
, m_pActiveDialog(NULL)
{
#ifdef _DEBUG
  setDebugName("GUIForumThreadBrowser");
#endif//_DEBUG
	core::rect<s32> sbRect = rect;

  // create the scrollbar element
	sbRect.UpperLeftCorner.Y = FORUM_HEADER_HEIGHT + FORUM_OPTIONBAR_HEIGHT;
	sbRect.LowerRightCorner.X -= 2;
	sbRect.UpperLeftCorner.X = sbRect.LowerRightCorner.X - FORUM_SCROLLBAR_WIDTH;
	sbRect.LowerRightCorner.Y = rect.getHeight();
	m_pSB = environment->addScrollBar(false, sbRect, this);
	m_pSB->setMax(0);
	m_pSB->setSubElement(true);
	m_pSB->setVisible(true);

	// create option buttons
	core::rect<s32> btnRect = AbsoluteRect;
	btnRect.LowerRightCorner.Y = btnRect.UpperLeftCorner.Y + FORUM_OPTIONBAR_HEIGHT;
	btnRect.UpperLeftCorner.X += 5;
	btnRect.UpperLeftCorner.Y += 5;
	btnRect.LowerRightCorner.X = btnRect.UpperLeftCorner.X + 100;
	btnRect.LowerRightCorner.Y -= 5;
  absToRelRect(this, btnRect);

  // the << BACK button
	m_optionBtns[0].bVisible = true;
  m_optionBtns[0].bPressed = false;
	m_optionBtns[0].rectBtn = btnRect;
	m_optionBtns[0].label = L"<< Back";

  // the NEW TOPIC button
	offsetRect(btnRect, btnRect.getWidth() + 10, 0);
	m_optionBtns[1].bVisible = true;
  m_optionBtns[1].bPressed = false;
	m_optionBtns[1].rectBtn = btnRect;
	m_optionBtns[1].label = L"New Topic";

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;
}

///////////////////////////////////////////////////////////////////////

GUIForumThreadBrowser::~GUIForumThreadBrowser(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::clearThreads()
{
  m_threads.erase( m_threads.begin(), m_threads.end() );
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

  // clear the threads vector
  clearThreads();

  const ForumThreadMap& threads = m_currentCategory->getThreads();
  ForumThreadMap::const_iterator it = threads.begin();
  ForumThreadMap::const_iterator limit = threads.end();

  for ( ; it != limit; it++ )
  {
    addThread( it->second );
  }

  // convert all category rectangles into relative co-ordinate rects
  if ( m_threads.size() )
  {
    ThreadVector::iterator it = m_threads.begin();
    ThreadVector::iterator limit = m_threads.end();

    for ( ; it != limit; it++ )
    {
      absToRelRect( this, it->rect );
    }
  }

  m_pSB->setMax(m_pixelOverflow);
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadBrowser::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
  case  EET_MOUSE_INPUT_EVENT:
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

      case  EMIE_LMOUSE_LEFT_UP:
        {
          bResult = OnLButtonUp(event.MouseInput);
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
            ThreadVector::iterator it = m_threads.begin();
            ThreadVector::iterator limit = m_threads.end();

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

bool GUIForumThreadBrowser::OnLButtonDown(const SEvent::SMouseInput& event)
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

  // check if any of the buttons was clicked
  core::rect<s32> itemRect;
  for ( int i = 0; i < 4; i++ )
  {
    itemRect = m_optionBtns[i].rectBtn;
    relToAbsRect(this, itemRect);
    if ( itemRect.isPointInside( position2d<s32>(event.X, event.Y) ) )
    {
      m_optionBtns[i].bPressed = true;
    }
    else
    {
      m_optionBtns[i].bPressed = false;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadBrowser::OnLButtonUp(const SEvent::SMouseInput& event)
{
  core::rect<s32> itemRect;

  // check if any of the buttons was clicked
  for ( int i = 0; i < 4; i++ )
  {
    if ( m_optionBtns[i].bPressed )
    {
      itemRect = m_optionBtns[i].rectBtn;
      relToAbsRect(this, itemRect);
      if ( itemRect.isPointInside( position2d<s32>(event.X, event.Y) ) )
      {
        // perform the necessary function here...
        OnOptionButtonPressed(i);
      }
    }
    m_optionBtns[i].bPressed = false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadBrowser::OnLButtonDblClick(const SEvent::SMouseInput& event)
{
	std::vector<ForumThreadStruct>::iterator it = m_threads.begin();
	std::vector<ForumThreadStruct>::iterator limit = m_threads.end();
  core::rect<s32> itemRect;

	for ( ; it != limit; it++ )
	{
    itemRect = it->rect;
    relToAbsRect(this, itemRect);
		if ( itemRect.isPointInside( position2d<s32>( event.X, event.Y ) ) )
		{
			if ( m_pSink )
				m_pSink->OnThreadSelected( m_currentCategory->getID(), it->pThread->getThreadID() );
			break;
		}
	}

	return true;
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
	drawOptionBar(pVideo);
  drawHeader(pVideo);
	
  AbsoluteClippingRect.UpperLeftCorner.Y += (FORUM_OPTIONBAR_HEIGHT + FORUM_HEADER_HEIGHT);

	// draw the threads
	ThreadVector::iterator it = m_threads.begin();
	ThreadVector::iterator limit = m_threads.end();

	for ( ; it != limit; it++ )
	{
		drawThread(*it);
	}

  AbsoluteClippingRect.UpperLeftCorner.Y -= (FORUM_OPTIONBAR_HEIGHT + FORUM_HEADER_HEIGHT);

  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::drawOptionBar(IVideoDriver* pVideo)
{
	core::rect<s32> bRect = AbsoluteRect,
									btnRect;
	SColor darkBlue(128, 0, 0, 255), black(255, 0, 0, 0), blueAlpha(128, 0, 0, 255), white(255, 255, 255, 255);

  // draw the bar background
	bRect.LowerRightCorner.Y = bRect.UpperLeftCorner.Y + FORUM_OPTIONBAR_HEIGHT;
	pVideo->draw2DRectangle( bRect, blueAlpha, blueAlpha, black, black );
	pVideo->draw2DLine( position2d<s32>(bRect.UpperLeftCorner.X, bRect.LowerRightCorner.Y-1),
											position2d<s32>(bRect.LowerRightCorner.X, bRect.LowerRightCorner.Y-1),
											SColor(255, 64, 64, 64) );

	// draw buttons
	IGUISkin* pSkin = Environment->getSkin();
	IGUIFont* pFont = pSkin->getFont();

	for ( int i = 0; i < 4; i++ )
	{
		if ( m_optionBtns[i].bVisible )
		{
      relToAbsRect(this, m_optionBtns[i].rectBtn);
      if ( m_optionBtns[i].bPressed )
        pSkin->draw3DButtonPanePressed(0, m_optionBtns[i].rectBtn);
      else
			  pSkin->draw3DButtonPaneStandard(0, m_optionBtns[i].rectBtn);
			pFont->draw(m_optionBtns[i].label.c_str(), m_optionBtns[i].rectBtn, black, true, true);
      absToRelRect(this, m_optionBtns[i].rectBtn);
		}
	}
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::drawHeader(IVideoDriver* pVideo)
{
  std::wstringstream ss;
  core::rect<s32> hRect = AbsoluteRect;
  IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkBlue(128, 0, 0, 255), black(255, 0, 0, 0), whiteAlpha(128, 255, 255, 255), white(255, 255, 255, 255);

	hRect.UpperLeftCorner.Y += FORUM_OPTIONBAR_HEIGHT;
  hRect.LowerRightCorner.Y = hRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT;
  pVideo->draw2DRectangle( hRect, darkBlue, darkBlue, whiteAlpha, whiteAlpha );
	pVideo->draw2DLine( position2d<s32>(hRect.UpperLeftCorner.X, hRect.UpperLeftCorner.Y),
										  position2d<s32>(hRect.LowerRightCorner.X, hRect.UpperLeftCorner.Y),
											SColor(128, 196, 196, 196) );
  // draw the header
  hRect.UpperLeftCorner.X += 10;

  ss << ResourceManager::instance().GetClientString( STR_APP_FORUM_CATEGORY ).c_str() << L": " << m_currentCategory->getName().c_str();

	drawStrokedFont( pFont, ss.str().c_str(), black, white, hRect, false, true );
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::drawThread(ForumThreadStruct& thread)
{
	std::wstringstream ss;
	IVideoDriver* pVideo = Environment->getVideoDriver();
  IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkGrey(255, 64, 64, 64), black(255, 0, 0, 0);
  core::rect<s32> cRect;

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

  relToAbsRect(this, thread.rect);
  cRect = thread.rect;

  if ( thread.rect.UpperLeftCorner.Y < AbsoluteClippingRect.LowerRightCorner.Y )
  {
	  // do a highlight if needed
	  if ( thread.bHighlighted )
		  pVideo->draw2DRectangle( thread.rect, darkGrey, darkGrey, black, black, &AbsoluteClippingRect );

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
	  cRect.UpperLeftCorner.X = cRect.LowerRightCorner.X - 170;
    if ( cRect.UpperLeftCorner.Y >= AbsoluteClippingRect.UpperLeftCorner.Y &&
        cRect.UpperLeftCorner.Y <= AbsoluteClippingRect.LowerRightCorner.Y )
    {
	    pVideo->draw2DLine( cRect.UpperLeftCorner, position2d<s32>(cRect.UpperLeftCorner.X, cRect.LowerRightCorner.Y), m_textColor );
    }
	  cRect.UpperLeftCorner.X += 10;
	  ss << thread.pThread->getDateCreated().c_str();
	  pFont->draw( ss.str().c_str(), cRect, m_textColor, false, true, &AbsoluteClippingRect );

	  // draw the bottom seperator
    if ( cRect.UpperLeftCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
      cRect.UpperLeftCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
    if ( cRect.LowerRightCorner.Y < AbsoluteClippingRect.UpperLeftCorner.Y )
      cRect.LowerRightCorner.Y = AbsoluteClippingRect.UpperLeftCorner.Y;
    if ( cRect.LowerRightCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
      cRect.LowerRightCorner.Y = AbsoluteClippingRect.LowerRightCorner.Y;
    pVideo->draw2DLine( thread.rect.UpperLeftCorner, position2d<s32>( thread.rect.LowerRightCorner.X, thread.rect.UpperLeftCorner.Y ) );
	  pVideo->draw2DLine( position2d<s32>( AbsoluteClippingRect.UpperLeftCorner.X, cRect.LowerRightCorner.Y ),
											  cRect.LowerRightCorner );
  }

  absToRelRect(this, thread.rect);
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::checkHighlights()
{
	// do the highlighting
	std::vector<ForumThreadStruct>::iterator it = m_threads.begin();
	std::vector<ForumThreadStruct>::iterator limit = m_threads.end();
  core::rect<s32> itemRect;

  AbsoluteClippingRect.UpperLeftCorner.Y += FORUM_HEADER_HEIGHT + FORUM_OPTIONBAR_HEIGHT;
	for ( ; it != limit; it++ )
	{
    itemRect = it->rect;
    relToAbsRect(this, itemRect);
		if ( AbsoluteClippingRect.isPointInside( m_mousePos ) &&
          itemRect.isPointInside( m_mousePos ) )
		{
			it->bHighlighted = true;
		}
		else
		{
			it->bHighlighted = false;
		}
	}
  AbsoluteClippingRect.UpperLeftCorner.Y -= (FORUM_HEADER_HEIGHT + FORUM_OPTIONBAR_HEIGHT);
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
    fts.rect.UpperLeftCorner = position2d<s32>(AbsoluteClippingRect.UpperLeftCorner.X, AbsoluteClippingRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT + FORUM_OPTIONBAR_HEIGHT);
    fts.rect.LowerRightCorner.Y = fts.rect.UpperLeftCorner.Y + FORUM_THREAD_ITEM_HEIGHT;
    fts.rect.LowerRightCorner.X = AbsoluteClippingRect.LowerRightCorner.X;
  }

  // check if the item's rect is overflowing
  if ( fts.rect.LowerRightCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
  {
    if ( fts.rect.UpperLeftCorner.Y > AbsoluteClippingRect.LowerRightCorner.Y )
      m_pixelOverflow += FORUM_THREAD_ITEM_HEIGHT;
    else
      m_pixelOverflow += fts.rect.LowerRightCorner.Y - AbsoluteClippingRect.LowerRightCorner.Y;
  }

	m_threads.push_back(fts);
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::OnOptionButtonPressed(int index)
{
  if ( index < 0 || index >= 4 )
    return;

  switch ( index )
  {
  case  0:      // the BACK button
    {
      if ( m_pSink )
        m_pSink->OnThreadViewClosed();
    }
    break;

  case  1:      // the NEW TOPIC button
		{
			OpenNewThreadWindow();
		}
    break;

  case  2:      // reserved
    break;

  case  3:      // reserved
    break;
  }
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::OpenNewThreadWindow()	
{
	DlgNewThread* pDlg = new DlgNewThread(Environment, L"New Thread", 0, -1);

	pDlg->setSuccessCallback( onNewThreadDlgComplete, (void*)this );
	pDlg->setCancelCallback( onNewThreadDlgCancel, (void*)this );
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::onNewThreadDlgComplete(void* pParam)
{
	GUIForumThreadBrowser* pThis = (GUIForumThreadBrowser*) pParam;
	DlgNewThread* pDlg = (DlgNewThread*)pThis->m_pActiveDialog;

	// get forum post information
	pDlg->getSubject();
	pDlg->getMessage();

	delete pDlg;
	pThis->m_pActiveDialog = NULL;
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadBrowser::onNewThreadDlgCancel(void* pParam)
{
	GUIForumThreadBrowser* pThis = (GUIForumThreadBrowser*) pParam;
	delete pThis->m_pActiveDialog;
}
