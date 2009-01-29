#include "FCDialog.h"
#error You need to fix the bugs in this class
FCDialog::FCDialog(IGUIEnvironment* pEnv)
: IGUIElement(EGUIET_ELEMENT, pEnv, 0, -1, core::rect<s32>(0, 0, 0, 0))
, m_pEnv(pEnv)
, m_pWindow(NULL)
, m_bModal(false)
{
}

///////////////////////////////////////////////////////////////////////

FCDialog::~FCDialog(void)
{
}

///////////////////////////////////////////////////////////////////////

bool FCDialog::Create(core::rect<s32> rect, const wchar_t* caption, bool bModal, IGUIElement* pParent, s32 id)
{
	if ( m_pWindow )
		return false;

	// remove ourselves from the current parent
	if ( getParent() )
		getParent()->removeChild(this);
	if ( pParent )
		pParent->addChild(this);

	if ( (m_pWindow = m_pEnv->addWindow( rect, bModal, caption, this, id )) )
	{
		AbsoluteRect = rect;
		addChild(m_pWindow);
	}

	return (m_pWindow != NULL);
}

///////////////////////////////////////////////////////////////////////

bool FCDialog::Create(s32 id, const char* layoutXmlFile, bool bModal, IGUIElement* pParent)
{
	if ( m_pWindow )
		return false;

	// remove ourselves from the current parent
	if ( getParent() )
		getParent()->removeChild(this);
	if ( pParent )
		pParent->addChild(this);

	if ( m_pEnv->loadGUI(layoutXmlFile, pParent) )
	{
		if ( !pParent )
			pParent = m_pEnv->getRootGUIElement();

		IGUIElement* pElem = pParent->getElementFromId(id);

		if ( pElem->getType() == EGUIET_WINDOW )
		{
			m_pWindow = (IGUIWindow*)pElem;
			AbsoluteRect = m_pWindow->getAbsolutePosition();
			addChild(m_pWindow);
		}
	}

	return ( m_pWindow != NULL );
}

///////////////////////////////////////////////////////////////////////

void FCDialog::GetClientRect(core::rect<s32>& rect)
{
  core::rect<s32> absRect = m_pWindow->getAbsolutePosition();
	rect.UpperLeftCorner.X = 3;
  rect.UpperLeftCorner.Y = 21;
  rect.LowerRightCorner.X = rect.UpperLeftCorner.X + absRect.getWidth() - 6;
  rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + absRect.getHeight() - 24;
}

///////////////////////////////////////////////////////////////////////

void FCDialog::CenterWindow()
{
  core::dimension2d<s32> screenSize = m_pEnv->getVideoDriver()->getScreenSize();
  core::rect<s32> wndSize = m_pWindow->getAbsolutePosition();
  core::position2d<s32> newWndPos;

  newWndPos.X = (screenSize.Width / 2) - wndSize.getWidth()/2;
  newWndPos.Y = (screenSize.Height / 2) - wndSize.getHeight()/2;
  m_pWindow->move(newWndPos);
}

///////////////////////////////////////////////////////////////////////

bool FCDialog::OnEvent(const SEvent& event)
{
  switch(event.EventType)
	{
	case EET_GUI_EVENT:
		switch(event.GUIEvent.EventType)
		{
		case EGET_ELEMENT_FOCUSED:
			if ( m_bModal )
			{
				// only children are allowed the focus
				if (event.GUIEvent.Caller != this && !isMyChild(event.GUIEvent.Caller))
					m_pEnv->setFocus(this);
			}
			return false;
		case EGET_ELEMENT_FOCUS_LOST:
			if ( m_bModal )
			{
				// only children are allowed the focus
				if (!(isMyChild(event.GUIEvent.Element) || event.GUIEvent.Element == this))
				{
					return true;
				}
				else
				{
					return IGUIElement::OnEvent(event);
				}
			}
			break;
		case EGET_ELEMENT_CLOSED:
			// do not interfere with children being removed
			return IGUIElement::OnEvent(event);
		default:
			break;
		}
		break;
	default:
		break;
	}
	
	IGUIElement::OnEvent(event);

	return (m_bModal == true); // absorb everything else if we're modal
}
