#include <ctime>
#include <sstream>
#include "DesktopAppBar.h"

DesktopAppBar::DesktopAppBar(IGUIEnvironment* env, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, env, pParent, id, core::rect<s32>(0, 0, 0, 0))
, m_height(24)
, m_pClockFont(NULL)
, m_pActiveApp(NULL)
{
  core::dimension2d<s32> screenSize = env->getVideoDriver()->getScreenSize();
  if ( getParent() )
  {
    screenSize.Width = getParent()->getAbsolutePosition().getWidth();
    screenSize.Height = getParent()->getAbsolutePosition().getHeight();
  }
  // app bar is currently positioned at the top of the screen (similar to OS X)
  core::rect<s32> rect(0, 0, screenSize.Width, m_height);
  AbsoluteClippingRect = AbsoluteRect = rect;

  // load the clock font
  m_pClockFont = Environment->getFont("./clientdata/fonts/fontfixedsys.xml");
  if ( !m_pClockFont )
    m_pClockFont = Environment->getBuiltInFont();
}

//////////////////////////////////////////////////////////////////////////////////////////

DesktopAppBar::~DesktopAppBar(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::setActiveApp(InGameAppWindow* pApp)
{
  m_pActiveApp = pApp;
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::draw()
{
  IVideoDriver* pVideo = Environment->getVideoDriver();

  // draw the app bar background
  SColor topColor(128, 164, 164, 164), bottomColor(128, 0, 0, 0), shade(128, 0, 0, 0);
  pVideo->draw2DRectangle(AbsoluteRect, topColor, topColor, bottomColor, bottomColor, &AbsoluteClippingRect); 

  // draw the Active App context
  if ( m_pActiveApp )
  {
    IGUIFont* pFont = Environment->getSkin()->getFont();

    if ( !pFont )
      pFont = m_pClockFont;

    core::rect<s32> cRect = AbsoluteRect;

    cRect.UpperLeftCorner.X += 10;
    pFont->draw( m_pActiveApp->getAppName(),
                 cRect,
                 SColor(255, 255, 255, 255),
                 false,
                 true );
  }

  // draw the timestamp
  if ( m_pClockFont )
  {
    core::rect<s32> tRect = AbsoluteRect;
    std::wstringstream ss;
    time_t tt = time(NULL);
    tm* t = localtime( &tt );

    ss << (t->tm_hour <= 9 ? L"0" : L"") << t->tm_hour << L":"
      << (t->tm_min <= 9 ? L"0" : L"") << t->tm_min;

    // setup the rectangle for the text
    tRect.UpperLeftCorner.X = tRect.LowerRightCorner.X - m_pClockFont->getDimension( ss.str().c_str() ).Width - 30;
    // draw a slight darker version of the app bar for the timer
    pVideo->draw2DRectangle( shade, tRect );
    // draw the offset shadow
    tRect.UpperLeftCorner.X += 3;
    tRect.UpperLeftCorner.Y += 2;
    m_pClockFont->draw(ss.str().c_str(), tRect, SColor(194, 0, 0, 0), true, true);
    // draw the text
    tRect.UpperLeftCorner.X -= 3;
    tRect.UpperLeftCorner.Y -= 2;
    m_pClockFont->draw(ss.str().c_str(), tRect, SColor(194, 255, 255, 255), true, true);
    // draw the seperator
    pVideo->draw2DLine( tRect.UpperLeftCorner, core::position2d<s32>( tRect.UpperLeftCorner.X, tRect.LowerRightCorner.Y ), SColor(194, 64, 64, 64) );
    pVideo->draw2DLine( core::position2d<s32>( tRect.UpperLeftCorner.X+1, tRect.UpperLeftCorner.Y), core::position2d<s32>( tRect.UpperLeftCorner.X+1, tRect.LowerRightCorner.Y ), SColor(128, 255, 255, 255) );
  }
}