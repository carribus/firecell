#include <sstream>
#include "FCController.h"
#include "FCViewevent.h"
#include "GUIFCCharacterItem.h"

#define CHARSEL_ITEM_HEIGHT         100
#define CHARSEL_ITEM_WIDTH          400

GUIFCCharacterItem::GUIFCCharacterItem(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, pEnv, pParent ? pParent : (IGUIElement*)pEnv, id, core::rect<s32>(0, 0, CHARSEL_ITEM_WIDTH, CHARSEL_ITEM_HEIGHT))
, m_pContainer(NULL)
, m_pDriver(NULL)
, m_pCharacter(NULL)
, m_bHighlight(false)
{
#ifdef _DEBUG
  setDebugName("GUIFCCharacterItem");
#endif//_DEBUG

  if ( pEnv )
    m_pDriver = pEnv->getVideoDriver();
}

///////////////////////////////////////////////////////////////////////

GUIFCCharacterItem::~GUIFCCharacterItem(void)
{
}

void GUIFCCharacterItem::grab()
{
  IGUIElement::grab();
}

bool GUIFCCharacterItem::drop()
{
  return IGUIElement::drop();
}

///////////////////////////////////////////////////////////////////////

void GUIFCCharacterItem::draw()
{
	if ( !IsVisible )
		return;

  SColor grey(255, 128, 128, 128);
	SColor white(255, 255, 255, 255);
  SColor charInset(255, 0, 64, 128);
  SColor blue(255, 64, 128, 255);
  core::rect<s32> rect = AbsoluteRect;
  wstringstream s;

  // draw the outline and background
  m_pDriver->draw2DRectangle( rect, grey, grey, grey, grey);

  rect.LowerRightCorner.X -= 2;
  rect.LowerRightCorner.Y -= 2;
  rect.UpperLeftCorner.X += 2;
  rect.UpperLeftCorner.Y += 2;
  if ( !m_bHighlight )
	  m_pDriver->draw2DRectangle( rect, white, white, white, white);
  else
	  m_pDriver->draw2DRectangle( rect, white, white, blue, blue);

  // draw the 'character image' inset
  rect.UpperLeftCorner.X += 7;
  rect.UpperLeftCorner.Y += 7;
  rect.LowerRightCorner.Y -= 7;
  rect.LowerRightCorner.X = rect.UpperLeftCorner.X + rect.getHeight();
  m_pDriver->draw2DRectangle( rect, charInset, charInset, charInset, charInset );

  // update rectangle for lines of text
	rect.UpperLeftCorner.X = rect.LowerRightCorner.X + 10;
	rect.LowerRightCorner.X = AbsoluteRect.LowerRightCorner.X - 10;
  // create the character details string and textobject
	IGUIFont* pFont = Environment->getSkin()->getFont();

	if ( !pFont )
		pFont = Environment->getBuiltInFont();

	wstring str = m_pCharacter->GetName();
	core::dimension2d<s32> extents = pFont->getDimension( str.c_str() );
	rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + extents.Height;

	pFont->draw( str.c_str(), rect, SColor(255, 0, 0, 0) );

	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y;
	rect.LowerRightCorner.Y += extents.Height;
	s << L"Level: " << m_pCharacter->GetLevel();
	str = s.str();
	pFont->draw( str.c_str(), rect, SColor(255, 0, 0, 0) );

	rect.UpperLeftCorner.Y = rect.LowerRightCorner.Y;
	rect.LowerRightCorner.Y += extents.Height;
	s.str(L"");
	s << L"XP: " << m_pCharacter->GetXP();
	str = s.str();
	pFont->draw( str.c_str(), rect, SColor(255, 0, 0, 0) );

	IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool GUIFCCharacterItem::OnEvent(const SEvent& event)
{
  bool bHandled = false;

  switch ( event.EventType )
  {
  case  EET_GUI_EVENT:
    {
      s32 elemID = event.GUIEvent.Caller->getID();

      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_HOVERED:
        {
          if ( event.GUIEvent.Caller == this )
          {
            SetHighlight(true);
            bHandled = true;
          }
        }
        break;

      case  EGET_ELEMENT_LEFT:
        {
          if ( event.GUIEvent.Caller == this )
          {
            SetHighlight(false);
            bHandled = true;
          }
        }
        break;

      default:
        break;
      }
    }
    break;

	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_LMOUSE_LEFT_UP:
				{
          if ( isPointInside( core::position2d<s32>( event.MouseInput.X, event.MouseInput.Y ) ) )
          {
						Character* pChar = GetCharacter();

						if ( m_pContainer && pChar )
						{
							FCViewEvent e( VE_CharacterSelected, pChar->GetID() );
							m_pContainer->GetController()->OnViewEvent(e);
              bHandled = true;
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

  return bHandled;
}