#include "AppletSysMon.h"

#define ICON_FILE "./clientdata/gfx/applets/sysmon.jpg"

AppletSysMon::AppletSysMon(DesktopAppBar* pParent, IGUIEnvironment* pEnv)
: m_pParent(pParent)
, m_pEnv(pEnv)
{
  // load the icon file
  IImage* pIcon = pEnv->getVideoDriver()->createImageFromFile(ICON_FILE);

  m_pIcon = pEnv->getVideoDriver()->addTexture(ICON_FILE, pIcon);
  pIcon->drop();
}

//////////////////////////////////////////////////////////////////////////////////////////

AppletSysMon::~AppletSysMon(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

std::wstring AppletSysMon::getHoverText()
{
  return L"System Monitor";
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::getAppletIconRect(core::rect<s32>& rect)
{
  if ( m_pIcon )
  {
    rect.UpperLeftCorner.X = rect.UpperLeftCorner.Y = 0;
    rect.LowerRightCorner.X = m_pIcon->getSize().Width;
    rect.LowerRightCorner.Y = m_pIcon->getSize().Height;
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::drawAppletIcon(core::rect<s32>& rect)
{
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
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::onAppletClicked()
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void AppletSysMon::OnAppletRightClicked()
{
}
