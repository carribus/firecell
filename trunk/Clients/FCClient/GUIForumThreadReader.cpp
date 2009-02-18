#include <sstream>
#include "../common/irrlichtUtil/irrfontfx.h"
#include "../common/irrlichtUtil/irrutils.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "DlgNewThread.h"
#include "GUIForumThreadReader.h"

#define FORUM_HEADER_HEIGHT                 0
#define FORUM_OPTIONBAR_HEIGHT              30
#define FORUM_SCROLLBAR_WIDTH               20

GUIForumThreadReader::GUIForumThreadReader(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
, m_pSink(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_pSB(NULL)
, m_pixelUnderflow(0)
, m_pActiveDialog(NULL)
, m_category_id(0)
, m_thread_id(0)
{
#ifdef _DEBUG
  setDebugName("GUIForumThreadReader");
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
	m_optionBtns[0].label = ResourceManager::instance().GetClientString(STR_APP_FORUM_OPTION_BACK).c_str();

  // the NEW TOPIC button
	offsetRect(btnRect, btnRect.getWidth() + 10, 0);
	m_optionBtns[1].bVisible = true;
  m_optionBtns[1].bPressed = false;
	m_optionBtns[1].rectBtn = btnRect;
	m_optionBtns[1].label = ResourceManager::instance().GetClientString(STR_APP_FORUM_OPTION_REPLY).c_str();

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;
}

///////////////////////////////////////////////////////////////////////

GUIForumThreadReader::~GUIForumThreadReader(void)
{
	clearPosts();
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::clearPosts()
{
	vector<IGUIElement*>::iterator it = m_posts.begin();
	vector<IGUIElement*>::iterator limit = m_posts.end();

	for ( ; it != limit; it++ )
	{
		this->removeChild( (*it) );
	}
	m_posts.erase( m_posts.begin(), m_posts.end() );
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::updateThreadContent(ForumThread* pThread)
{
  if ( !m_pModel )
    return;
  if ( !pThread )
    return;
  if ( m_category_id != pThread->getCategoryID() || m_thread_id != pThread->getThreadID() )
    return;

	clearPosts();

  core::rect<s32> r = core::rect<s32>(5, 5 + FORUM_HEADER_HEIGHT + FORUM_OPTIONBAR_HEIGHT, AbsoluteClippingRect.getWidth()-5, AbsoluteClippingRect.getHeight());

  // populate the main thread data
	addThreadPost(pThread, r, SColor(128, 64, 64, 128), m_textColor);

  ForumThread::ThreadVector& posts = pThread->getAllPosts();
  ForumThread::ThreadVector::iterator it = posts.begin();
  ForumThread::ThreadVector::iterator limit = posts.end();

  for ( ; it != limit; it++ )
  {
    offsetRect(r, 0, r.getHeight()+10);
		addThreadPost( *it, r, SColor(128, 64, 64, 128), m_textColor );
  }
	m_posts.size();

  m_pSB->setMax( r.LowerRightCorner.Y - RelativeRect.getHeight() );
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadReader::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
  case  EET_MOUSE_INPUT_EVENT:
    {
			switch ( event.MouseInput.Event )
			{
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
      case  EGET_SCROLL_BAR_CHANGED:
        {
          vector<IGUIElement*>::iterator it = m_posts.begin();
          vector<IGUIElement*>::iterator limit = m_posts.end();
          core::rect<s32> r;

          s32 diff = m_pSB->getPos()-m_pixelUnderflow;

          for ( ; it != limit; it++ )
          {
            r = (*it)->getRelativePosition();
            offsetRect(r, 0, -diff);
            (*it)->setRelativePosition(r);
          }

          m_pixelUnderflow = m_pSB->getPos();
          bResult = true;
        }
        break;

      default:
        break;
      }
    }
    break;

	case	EET_USER_EVENT:
		{
			if ( event.UserEvent.UserData1 == 1 )				// mission accepted
			{
				if ( event.UserEvent.UserData2 )
				{
					if ( m_pSink )
						m_pSink->OnThreadMissionAccept( event.UserEvent.UserData2 );
				}
			}
		}
		break;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadReader::OnLButtonDown(const SEvent::SMouseInput& event)
{
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

bool GUIForumThreadReader::OnLButtonUp(const SEvent::SMouseInput& event)
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

void GUIForumThreadReader::draw()
{
  if ( !IsVisible )
    return;

  IVideoDriver* pVideo = Environment->getVideoDriver();
  IGUIFont* pFont = Environment->getSkin()->getFont();

  pVideo->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );

  IGUIElement::draw();

	drawOptionBar(pVideo);
	drawHeader(pVideo);
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::drawOptionBar(IVideoDriver* pVideo)
{
	core::rect<s32> bRect = AbsoluteRect,
									btnRect;
	SColor darkBlue(255, 0, 0, 128), black(255, 0, 0, 0), blueAlpha(255, 0, 0, 255), white(255, 128, 128, 128);

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

void GUIForumThreadReader::drawHeader(IVideoDriver* pVideo)
{
  std::wstringstream ss;
  core::rect<s32> hRect = AbsoluteRect;
  IGUIFont* pFont = Environment->getSkin()->getFont();
  SColor darkBlue(255, 0, 0, 128), black(255, 0, 0, 0), whiteAlpha(255, 128, 128, 128), white(255, 128, 128, 128);

	hRect.UpperLeftCorner.Y += FORUM_OPTIONBAR_HEIGHT;
  hRect.LowerRightCorner.Y = hRect.UpperLeftCorner.Y + FORUM_HEADER_HEIGHT;
  pVideo->draw2DRectangle( hRect, darkBlue, darkBlue, whiteAlpha, whiteAlpha );
	pVideo->draw2DLine( position2d<s32>(hRect.UpperLeftCorner.X, hRect.UpperLeftCorner.Y),
										  position2d<s32>(hRect.LowerRightCorner.X, hRect.UpperLeftCorner.Y),
											SColor(128, 196, 196, 196) );
  // draw the header
  hRect.UpperLeftCorner.X += 10;

	if ( m_posts.size() )
	{
/*
		_ThreadStruct ts = m_posts[0];
		ss << ResourceManager::instance().GetClientString( STR_APP_FORUM_THREAD_TITLE ).c_str() << L": " << ts.pThread->getTitle().c_str();
*/
	}

	drawStrokedFont( pFont, ss.str().c_str(), black, white, hRect, false, true );
}

///////////////////////////////////////////////////////////////////////
#include "GUIForumPostItem.h"
void GUIForumThreadReader::addThreadPost(ForumThread* pPost, core::rect<s32>& rect, SColor colBkg, SColor colText)
{
  std::wstringstream ss;
	GUIForumPostItem* pItem = new GUIForumPostItem(Environment, this, rect, colText, colBkg);

	if ( pItem )
	{
		pItem->setItemData(pPost);
		rect = pItem->getRelativePosition();
		m_posts.push_back(pItem);
	}

/*
  _ThreadStruct ts;

	ts.pThread = pPost;
  ss << pPost->getContent().c_str();
  ts.pTxtContent = Environment->addStaticText( ss.str().c_str(), rect, true, true, this );
	ts.pTxtContent->setBackgroundColor( colBkg );
  rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + ts.pTxtContent->getTextHeight();
  ts.pTxtContent->setRelativePosition( rect );
  ts.pTxtContent->setOverrideColor( colText );
  m_posts.push_back(ts);
*/
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::OnOptionButtonPressed(int index)
{
  if ( index < 0 || index >= 4 )
    return;

  switch ( index )
  {
  case  0:      // the BACK button
    {
			if ( m_pSink )
				m_pSink->OnThreadReaderViewClosed();
    }
    break;

  case  1:      // the REPLY button
		{
			OpenNewReplyWindow();
		}
    break;

  case  2:      // reserved
    break;

  case  3:      // reserved
    break;
  }
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::OpenNewReplyWindow()
{
	DlgNewThread* pDlg = new DlgNewThread(Environment, L"Post Reply", 0, -1);

	pDlg->setSuccessCallback( onReplyDlgComplete, (void*)this );
	pDlg->setCancelCallback( OnReplyDlgCancel, (void*)this );
  m_pActiveDialog = pDlg;
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::onReplyDlgComplete(void* pParam)
{
	GUIForumThreadReader* pThis = (GUIForumThreadReader*) pParam;
	DlgNewThread* pDlg = (DlgNewThread*)pThis->m_pActiveDialog;

  if ( pThis->m_pSink )
    pThis->m_pSink->OnThreadReply( pThis->m_category_id, pThis->m_thread_id, pDlg->getSubject(), pDlg->getMessage() );

	pThis->m_pActiveDialog = NULL;
}

///////////////////////////////////////////////////////////////////////

void GUIForumThreadReader::OnReplyDlgCancel(void* pParam)
{
	GUIForumThreadReader* pThis = (GUIForumThreadReader*) pParam;

	pThis->m_pActiveDialog = NULL;
}
