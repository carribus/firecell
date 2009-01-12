#include "GUIFCCharacterItem.h"

GUIFCCharacterItem::GUIFCCharacterItem(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, pEnv, pParent ? pParent : (IGUIElement*)pEnv, id, core::rect<s32>(0, 0, 200, 100))
{
	m_position.X = m_position.Y = 0;
	setText( L"THIS IS A GUIFCCHARACTERITEM OBJECT!!!" );
}

///////////////////////////////////////////////////////////////////////

GUIFCCharacterItem::~GUIFCCharacterItem(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIFCCharacterItem::draw()
{
	if ( !IsVisible )
		return;

	irr::video::IVideoDriver* pDriver = Environment->getVideoDriver();
	irr::video::SColor white(64, 255, 255, 255);

	this->setRelativePosition( m_position );
	pDriver->draw2DRectangle( AbsoluteRect, white, white, white, white);


	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

void GUIFCCharacterItem::SetPosition(s32 left, s32 top)
{
	m_position.X = left;
	m_position.Y = top;
}