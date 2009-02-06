#include "../common/irrlichtUtil/irrfontfx.h"
#include "DesktopIcon.h"

DesktopIcon::DesktopIcon(IDesktop* pDesktop, IGUIEnvironment* env, IGUIElement* pParent, const wchar_t* text, s32 id)
: IGUIElement(EGUIET_ELEMENT, env, (pParent?pParent:env->getRootGUIElement()), id, rect<s32>(0, 0, 0, 0) )
, m_pDesktop(pDesktop)
, m_pTexture(NULL)
, m_pFont(NULL)
, m_bSelected(false)
{
#ifdef _DEBUG
  setDebugName("DesktopIcon");
#endif//_DEBUG
  Text = text;
}

///////////////////////////////////////////////////////////////////////

DesktopIcon::~DesktopIcon(void)
{
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::setIcon(const std::string& filename)
{
	IImage* pIcon = Environment->getVideoDriver()->createImageFromFile(filename.c_str());
	
  m_pTexture = Environment->getVideoDriver()->addTexture(filename.c_str(), pIcon);
  calculateDimensions();
  pIcon->drop();
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::setText(const wchar_t* text)
{
  Text = text;
  calculateDimensions();
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::setWidth(s32 width)
{
  AbsoluteRect.LowerRightCorner.X = AbsoluteRect.UpperLeftCorner.X + width;
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::setHeight(s32 height)
{
  AbsoluteRect.LowerRightCorner.Y = AbsoluteRect.UpperLeftCorner.Y + height;
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::moveTo(s32 x, s32 y)
{
  rect<s32> oldRect = AbsoluteRect;
  AbsoluteClippingRect = AbsoluteRect = rect<s32>(x, y, x + oldRect.getWidth(), y + oldRect.getHeight());
}

///////////////////////////////////////////////////////////////////////

void DesktopIcon::draw()
{
  IVideoDriver* pVideo = Environment->getVideoDriver();
  dimension2d<s32> iconDim;
  position2d<s32> iconPos;

	iconDim = m_pTexture->getSize();
	iconPos.X = AbsoluteRect.UpperLeftCorner.X + (AbsoluteRect.getWidth()/2 - iconDim.Width/2);
	iconPos.Y = AbsoluteRect.UpperLeftCorner.Y;

  if ( m_bSelected )
  {
		pVideo->draw2DImage( m_pTexture, 
												 iconPos, 
												 rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
												 0, 
												 SColor(255, 255, 255, 255), 
												 true );
  }
  else
  {
	  pVideo->draw2DImage( m_pTexture, 
											   iconPos, 
											   rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
											   0, 
											   SColor(128, 255, 255, 255), 
											   true );
  }

	rect<s32> r = core::rect<s32>( AbsoluteRect.UpperLeftCorner.X, AbsoluteRect.UpperLeftCorner.Y + iconDim.Height + 4, AbsoluteRect.LowerRightCorner.X, AbsoluteRect.LowerRightCorner.Y );
	drawStrokedFont(m_pFont, Text.c_str(), SColor(128, 0, 0, 0), SColor(255, 255, 255, 255), r, true, true);
}

///////////////////////////////////////////////////////////////////////

bool DesktopIcon::OnEvent(const SEvent& event)
{
  bool bResult = false;

  switch ( event.EventType )
  {
  case  EET_MOUSE_INPUT_EVENT:
    {
      switch ( event.MouseInput.Event )
      {
      case  EMIE_LMOUSE_PRESSED_DOWN:
        m_bSelected = true;
				if ( m_pDesktop )
					m_pDesktop->OnDesktopIconSelected(this);
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

rect<s32> DesktopIcon::calculateDimensions()
{
  if ( m_pFont )
  {
    core::dimension2d<s32> txtExtents = m_pFont->getDimension( Text.c_str() );
		AbsoluteClippingRect = 
    AbsoluteRect = core::rect<s32>(AbsoluteRect.UpperLeftCorner.X, 
									     				    AbsoluteRect.UpperLeftCorner.Y, 
													        AbsoluteRect.UpperLeftCorner.X + (txtExtents.Width > m_pTexture->getSize().Width ? txtExtents.Width : m_pTexture->getSize().Width), 
													        AbsoluteRect.UpperLeftCorner.X + m_pTexture->getSize().Height + txtExtents.Height + 4);
  }
  return AbsoluteRect;
}