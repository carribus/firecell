#include "AppBarAppletBase.h"

AppBarAppletBase::AppBarAppletBase(DesktopAppBar* pParent, IGUIEnvironment* pEnv, const char* iconFilename)
: IGUIElement(EGUIET_ELEMENT, pEnv, (IGUIElement*)pParent, -1, core::rect<s32>(0, 0, 0, 0))
, m_pParent(pParent)
, m_pEnv(pEnv)
{
	#ifdef _DEBUG
	setDebugName("AppBarAppletBase");
	#endif
  // load the icon file
  if ( iconFilename )
  {
    IImage* pIcon = pEnv->getVideoDriver()->createImageFromFile(iconFilename);

    m_pIcon = pEnv->getVideoDriver()->addTexture(iconFilename, pIcon);
    pIcon->drop();
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

AppBarAppletBase::~AppBarAppletBase(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppBarAppletBase::getAppletIconRect(core::rect<s32>& rect)
{
  if ( m_pIcon && rect.getWidth() )
  {
    rect.UpperLeftCorner.X = rect.LowerRightCorner.X - m_pIcon->getSize().Width;
    rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + m_pIcon->getSize().Height;
    
    AbsoluteRect = AbsoluteClippingRect = rect;
  }
  else
    rect = AbsoluteRect;
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppBarAppletBase::drawAppletIcon(core::rect<s32>& rect)
{
/*
  IVideoDriver* pVideo = m_pEnv->getVideoDriver();
  dimension2d<s32> iconDim;
  position2d<s32> iconPos;

	iconDim = m_pIcon->getSize();
	iconPos.X = rect.UpperLeftCorner.X + (rect.getWidth()/2 - iconDim.Width/2);
	iconPos.Y = rect.UpperLeftCorner.Y;

  pVideo->draw2DImage( m_pIcon, 
										   iconPos, 
                       core::rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
										   0, 
										   SColor(255, 255, 255, 255), 
										   false );
*/
}

//////////////////////////////////////////////////////////////////////////////////////////

bool AppBarAppletBase::OnEvent(const SEvent& event)
{
	switch ( event.EventType )
	{
  case  EET_GUI_EVENT:
    {
      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_LEFT:
        {
          onAppletHover(false);
          return true;
        }
        break;

      case  EGET_ELEMENT_HOVERED:
        {
          onAppletHover(true);
          return true;
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

	return Parent ? Parent->OnEvent(event) : false;
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppBarAppletBase::draw()
{
  IVideoDriver* pVideo = m_pEnv->getVideoDriver();
  dimension2d<s32> iconDim;
  position2d<s32> iconPos;

	iconDim = m_pIcon->getSize();

  pVideo->draw2DImage( m_pIcon, 
										   AbsoluteRect.UpperLeftCorner, 
                       core::rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
										   0, 
										   SColor(255, 255, 255, 255), 
										   false );
}