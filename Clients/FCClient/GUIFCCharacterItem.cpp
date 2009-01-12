#include "GUIFCCharacterItem.h"

#define CHARSEL_ITEM_HEIGHT         100
#define CHARSEL_ITEM_WIDTH          400

GUIFCCharacterItem::GUIFCCharacterItem(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, pEnv, pParent ? pParent : (IGUIElement*)pEnv, id, core::rect<s32>(0, 0, CHARSEL_ITEM_WIDTH, CHARSEL_ITEM_HEIGHT))
, m_pDriver(NULL)
, m_pCharacter(NULL)
{
  if ( pEnv )
    m_pDriver = pEnv->getVideoDriver();

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

  SColor grey(255, 128, 128, 128);
	SColor white(255, 255, 255, 255);
  SColor charInset(255, 0, 64, 128);
  core::rect<s32> rect = AbsoluteRect;

  // draw the outline and background
	m_pDriver->draw2DRectangle( rect, grey, grey, grey, grey);
  rect.LowerRightCorner.X -= 2;
  rect.LowerRightCorner.Y -= 2;
  rect.UpperLeftCorner.X += 2;
  rect.UpperLeftCorner.Y += 2;
	m_pDriver->draw2DRectangle( rect, white, white, white, white);

  // draw the 'character image' inset
  rect.UpperLeftCorner.X += 7;
  rect.UpperLeftCorner.Y += 7;
  rect.LowerRightCorner.Y -= 7;
  rect.LowerRightCorner.X = rect.UpperLeftCorner.X +  rect.getHeight();
  m_pDriver->draw2DRectangle( rect, charInset, charInset, charInset, charInset );

  // localise the text rectangle to this element's rectangle
  rect.UpperLeftCorner.X = (rect.LowerRightCorner.X + 10) - AbsoluteRect.UpperLeftCorner.X;
  rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - 10 - AbsoluteRect.UpperLeftCorner.X;
  rect.UpperLeftCorner.Y -= AbsoluteRect.UpperLeftCorner.Y;
  rect.LowerRightCorner.Y -= AbsoluteRect.UpperLeftCorner.Y;
  IGUIStaticText* pTxtName = Environment->addStaticText( m_pCharacter->GetName().c_str(), rect, false, false, this );


	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////