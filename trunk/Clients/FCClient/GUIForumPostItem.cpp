#include <sstream>
#include "../common/irrlichtUtil/irrutils.h"
#include "GUIForumPostItem.h"

#define FPI_AUTHOR_HEIGHT				20
#define FPI_SUBJECT_HEIGHT			20

GUIForumPostItem::GUIForumPostItem(IGUIEnvironment* environment, IGUIElement* pParent, core::rect<s32> rect, SColor textCol, SColor bkgCol, s32 id)
: IGUIElement(EGUIET_ELEMENT, environment, pParent, id, rect)
, m_colText(textCol)
, m_colBkg(bkgCol)
, m_pTxtAuthor(NULL)
, m_pTxtSubject(NULL)
, m_pTxtContent(NULL)
{
#ifdef _DEBUG
  setDebugName("GUIForumPostItem");
#endif//_DEBUG
}

///////////////////////////////////////////////////////////////////////

GUIForumPostItem::~GUIForumPostItem(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIForumPostItem::setItemData(ForumThread* pThread)
{
	std::wstringstream ss;
	core::rect<s32> itemRect(5, 0, RelativeRect.getWidth()-5, RelativeRect.getHeight()), startRect;

	startRect = itemRect;

	itemRect.LowerRightCorner.Y = itemRect.UpperLeftCorner.Y + FPI_AUTHOR_HEIGHT;
	if ( !m_pTxtAuthor )
	{
		m_pTxtAuthor = Environment->addStaticText(L"Author: ...", itemRect, false, false, this);
//		m_pTxtAuthor->setBackgroundColor(m_colBkg);
		m_pTxtAuthor->setOverrideColor(m_colText);
		m_pTxtAuthor->setTextAlignment( EGUIA_UPPERLEFT, EGUIA_CENTER );
	}

	if ( pThread->getTitle().size() )
	{
 		offsetRect(itemRect, 0, itemRect.getHeight() + 5);
		if ( !m_pTxtSubject )
		{
			ss << L"Subject: " << pThread->getTitle().c_str();
			m_pTxtSubject = Environment->addStaticText(ss.str().c_str(), itemRect, false, false, this);
	//		m_pTxtSubject->setBackgroundColor(m_colBkg);
			m_pTxtSubject->setOverrideColor(m_colText);
			m_pTxtSubject->setTextAlignment( EGUIA_UPPERLEFT, EGUIA_CENTER );
			ss.str(L"");
		}
	}

	offsetRect(itemRect, 0, itemRect.getHeight() + 5);
	if ( !m_pTxtContent )
	{
		ss << pThread->getContent().c_str();
		m_pTxtContent = Environment->addStaticText(ss.str().c_str(), itemRect, true, true, this);
		m_pTxtContent->setBackgroundColor(m_colBkg);
		m_pTxtContent->setOverrideColor(m_colText);
	  itemRect.LowerRightCorner.Y = itemRect.UpperLeftCorner.Y + m_pTxtContent->getTextHeight();
		m_pTxtContent->setRelativePosition( itemRect );
	}

	core::rect<s32> myRect = getRelativePosition();
	myRect.LowerRightCorner.Y = myRect.UpperLeftCorner.Y + ( itemRect.LowerRightCorner.Y - startRect.UpperLeftCorner.Y ) + 5;
	setRelativePosition(myRect);
}

///////////////////////////////////////////////////////////////////////

void GUIForumPostItem::draw()
{
	if ( !IsVisible )
		return;

	IVideoDriver* pVideo = Environment->getVideoDriver();
	SColor black(255, 0, 0, 0), darkBlue(255, 0, 0, 128);

	pVideo->draw2DRectangle( AbsoluteRect, black, black, darkBlue, darkBlue, &AbsoluteClippingRect );

	IGUIElement::draw();
}