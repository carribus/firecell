#include <sstream>
#include "../common/irrlichtUtil/irrutils.h"
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "GUIForumThreadReader.h"

#define FORUM_HEADER_HEIGHT                 0
#define FORUM_OPTIONBAR_HEIGHT              0
#define FORUM_SCROLLBAR_WIDTH               20

GUIForumThreadReader::GUIForumThreadReader(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_pModel(NULL)
, m_backColor(backColor)
, m_textColor(textColor)
, m_pSB(NULL)
, m_pixelUnderflow(0)
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

	AbsoluteClippingRect.LowerRightCorner.X -= FORUM_SCROLLBAR_WIDTH;
}

///////////////////////////////////////////////////////////////////////

GUIForumThreadReader::~GUIForumThreadReader(void)
{
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

  core::rect<s32> r = core::rect<s32>(0, 0, AbsoluteClippingRect.getWidth(), AbsoluteClippingRect.getHeight());
  std::wstringstream ss;
  _ThreadStruct ts;

  // populate the main thread data
  ts.pThread = pThread;
  ss << pThread->getContent().c_str();
  ts.pTxtContent = Environment->addStaticText( ss.str().c_str(), r, true, true, this );
  r.LowerRightCorner.Y = r.UpperLeftCorner.Y + ts.pTxtContent->getTextHeight();
  ts.pTxtContent->setRelativePosition( r );
  ts.pTxtContent->setOverrideColor( m_textColor );
  m_posts.push_back(ts);

  ForumThread::ThreadVector& posts = pThread->getAllPosts();
  ForumThread::ThreadVector::iterator it = posts.begin();
  ForumThread::ThreadVector::iterator limit = posts.end();

  for ( ; it != limit; it++ )
  {
    offsetRect(r, 0, r.getHeight()+5);
    ss.str(L"");
    ts.pThread = (*it);
    ss << ts.pThread->getContent().c_str();
    ts.pTxtContent = Environment->addStaticText(ss.str().c_str(), r, true, true, this);
    r.LowerRightCorner.Y = r.UpperLeftCorner.Y + ts.pTxtContent->getTextHeight();
    ts.pTxtContent->setRelativePosition( r );
    ts.pTxtContent->setOverrideColor( m_textColor );
    m_posts.push_back(ts);
  }

  m_pSB->setMax( r.LowerRightCorner.Y - RelativeRect.getHeight() );
}

///////////////////////////////////////////////////////////////////////

bool GUIForumThreadReader::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
	case	EET_GUI_EVENT:
		{
			switch ( event.GUIEvent.EventType )
			{
      case  EGET_SCROLL_BAR_CHANGED:
        {
          vector<_ThreadStruct>::iterator it = m_posts.begin();
          vector<_ThreadStruct>::iterator limit = m_posts.end();
          core::rect<s32> r;

          s32 diff = m_pSB->getPos()-m_pixelUnderflow;

          for ( ; it != limit; it++ )
          {
            r = it->pTxtContent->getRelativePosition();
            offsetRect(r, 0, -diff);
            it->pTxtContent->setRelativePosition(r);
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
  }

  return bResult;
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
}
