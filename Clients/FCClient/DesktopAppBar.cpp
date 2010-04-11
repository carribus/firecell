#include <ctime>
#include <sstream>
#include "../common/ResourceManager.h"
#include "../common/irrlichtUtil/irrfontfx.h"
#include "clientdefs.h"
#include "clientstrings.h"

// applets
#include "AppletSysMon.h"

#include "DesktopAppBar.h"

DesktopAppBar::DesktopAppBar(IGUIEnvironment* env, IGUIElement* pParent, s32 id)
: IGUIElement(EGUIET_ELEMENT, env, pParent, id, core::rect<s32>(0, 0, 0, 0))
, m_height(24)
, m_pClockFont(NULL)
, m_pActiveApp(NULL)
, m_colShaderNormal(128, 0, 0, 0)
, m_colShaderHilite(64, 0, 0, 194)
{
#ifdef _DEBUG
  setDebugName("DesktopAppBar");
#endif//_DEBUG

  core::dimension2d<s32> screenSize = (env->getVideoDriver()->getScreenSize());
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

	// create the system app bar option
	// check if we need to add the rect to the appbar options vector
  IGUIFont* pFont = Environment->getSkin()->getFont();
	AppBarOption abo;

	if ( !pFont )
		pFont = m_pClockFont;

	abo.id = 0;
	abo.bAppOption = false;
	abo.bHighlight = false;
	abo.str = ResourceManager::instance().GetClientString( STR_APP_APPBAR_SYSTEM );
	abo.rect = AbsoluteRect;
	dimension2d<s32> txtExtents = pFont->getDimension( abo.str.c_str() );
	abo.rect.LowerRightCorner.X = abo.rect.UpperLeftCorner.X + txtExtents.Width + 30;
	
	m_appBarOptions.push_back(abo);

  // create the applets
  m_applets.push_back( new AppletSysMon(this, env) );

  updateAppletLayout();
}

//////////////////////////////////////////////////////////////////////////////////////////

DesktopAppBar::~DesktopAppBar(void)
{
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::setActiveApp(InGameAppWindow* pApp)
{
	if ( pApp != m_pActiveApp )
	{
		if ( m_pActiveApp )
			removeApplicationOptions();

		m_pActiveApp = pApp;
		if ( m_pActiveApp )
			addApplicationOptions();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::draw()
{
  IVideoDriver* pVideo = Environment->getVideoDriver();

  // draw the app bar background
  SColor topColor(128, 164, 164, 164), bottomColor(128, 0, 0, 0);
  pVideo->draw2DRectangle(AbsoluteRect, topColor, topColor, bottomColor, bottomColor, &AbsoluteClippingRect); 

	drawSystemContext(pVideo);
	drawActiveAppContext();
	drawClock(pVideo);
//  drawSystemTrayIcons(pVideo);

  IGUIElement::draw();
}

//////////////////////////////////////////////////////////////////////////////////////////

bool DesktopAppBar::OnEvent(const SEvent& event)
{
	switch ( event.EventType )
	{
	case	EET_GUI_EVENT:
		{
      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_LEFT:
        {
          // clear the highlights
					AppBarOptionVector::iterator it = m_appBarOptions.begin();

					while ( it != m_appBarOptions.end() )
					{
						it->bHighlight = false;
						it++;
					}
        }
        return true;

      case  EGET_ELEMENT_HOVERED:
        {
 					AppBarOptionVector::iterator it = m_appBarOptions.begin();

					while ( it != m_appBarOptions.end() )
					{
						if ( it->rect.isPointInside( position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
						{
							it->bHighlight = true;
						}
						else
							it->bHighlight = false;
						it++;
					}
/*
          if ( !bHandled )
          {
            core::rect<s32> aRect;
            std::wstring hoverText;
            AppBarApplets::iterator it = m_applets.begin();

            setToolTipText(L"");
            while ( it != m_applets.end() )
            {
              (*it)->getAppletIconRect(aRect);
              if ( aRect.isPointInside( position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
              {
                hoverText = (*it)->getHoverText();
                setToolTipText(hoverText.c_str());
                break;
              }
              ++it;
            }
          }
*/
        }
        return true;

      default:
        break;
      }
		}
		break;

	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_MOUSE_MOVED:
				{
					AppBarOptionVector::iterator it = m_appBarOptions.begin();

					while ( it != m_appBarOptions.end() )
					{
						if ( it->rect.isPointInside( position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
						{
							it->bHighlight = true;
						}
						else
							it->bHighlight = false;
						it++;
					}
				}
				return true;

			case	EMIE_LMOUSE_LEFT_UP:
				{
					AppBarOptionVector::iterator it = m_appBarOptions.begin();

					while ( it != m_appBarOptions.end() ) //&& !bHandled )
					{
						if ( it->rect.isPointInside( position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
						{
							switch ( it->id )
							{
							case	0:					// the system option
								showSystemMenu();
								break;

							default:
								break;
							}
						}
						it++;
					}
				}
				return true;

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

void DesktopAppBar::drawSystemContext(IVideoDriver* pVideo)
{
  IGUIFont* pFont = Environment->getSkin()->getFont();
	std::wstring str = ResourceManager::instance().GetClientString( STR_APP_APPBAR_SYSTEM );
	AppBarOption& abo = m_appBarOptions[0];
	rect<s32> rect = abo.rect;

  if ( !pFont )
    pFont = m_pClockFont;

	// shade the background
	pVideo->draw2DRectangle( (abo.bHighlight ? m_colShaderHilite : m_colShaderNormal), rect );
  // draw the seperator
	pVideo->draw2DLine( core::position2d<s32>( rect.LowerRightCorner.X, rect.UpperLeftCorner.Y ), 
										  core::position2d<s32>( rect.LowerRightCorner.X, rect.LowerRightCorner.Y ), SColor(194, 64, 64, 64) );
	pVideo->draw2DLine( core::position2d<s32>( rect.LowerRightCorner.X+1, rect.UpperLeftCorner.Y), 
											core::position2d<s32>( rect.LowerRightCorner.X+1, rect.LowerRightCorner.Y ), SColor(64, 255, 255, 255) );

	// draw the text
	drawStrokedFont( pFont, abo.str.c_str(), SColor(128, 0, 0, 0), SColor(194, 255, 255, 255), rect, true, true ); 
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawActiveAppContext()
{
  // draw the Active App context
  if ( m_pActiveApp )
  {
    IGUIFont* pFont = Environment->getSkin()->getFont();

    if ( !pFont )
      pFont = m_pClockFont;

		// iterate through the application context options
		AppBarOptionVector::iterator it = m_appBarOptions.begin() + 1;

		for ( ; it != m_appBarOptions.end(); it++ )
		{
			pFont->draw( it->str.c_str(),
									 it->rect,
									 SColor(255, 255, 255, 255),
									 true,
									 true );
		}
  }
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawSystemTrayIcons(IVideoDriver* pVideo)
{
  core::rect<s32> tRect = AbsoluteRect, aRect;
	AppBarOption& abo = m_appBarOptions[0];
  AppBarApplets::iterator it = m_applets.begin();
  AppBarApplets::iterator limit = m_applets.end();
  
  // setyp the rect for the system tray area
  tRect.LowerRightCorner.X = m_rectClock.UpperLeftCorner.X;
  tRect.UpperLeftCorner.X = abo.rect.LowerRightCorner.X+2;
  aRect = tRect;

  for ( ; it != limit; ++it )
  {
    (*it)->getAppletIconRect(aRect);
    tRect.LowerRightCorner.X = aRect.UpperLeftCorner.X;
    (*it)->drawAppletIcon(aRect);
  }

#if 0
  // draw the background of the valid region for the icons...
  pVideo->draw2DRectangle( SColor(128,255,0,0), tRect );
#endif
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawClock(IVideoDriver* pVideo)
{
	SColor shade(128, 0, 0, 0);

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
    m_rectClock = tRect;
    // draw a slight darker version of the app bar for the timer
    pVideo->draw2DRectangle( m_colShaderNormal, tRect );
    // draw the text
		drawStrokedFont( m_pClockFont, ss.str().c_str(), SColor(194, 0, 0, 0), SColor(194, 255, 255, 255), tRect, true, true );
    // draw the seperator
    pVideo->draw2DLine( tRect.UpperLeftCorner, core::position2d<s32>( tRect.UpperLeftCorner.X, tRect.LowerRightCorner.Y ), SColor(194, 64, 64, 64) );
    pVideo->draw2DLine( core::position2d<s32>( tRect.UpperLeftCorner.X+1, tRect.UpperLeftCorner.Y), core::position2d<s32>( tRect.UpperLeftCorner.X+1, tRect.LowerRightCorner.Y ), SColor(128, 255, 255, 255) );
  }

  updateAppletLayout();
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::addApplicationOptions()
{
	if ( m_pActiveApp )
	{
    IGUIFont* pFont = Environment->getSkin()->getFont();
		dimension2d<s32> txtExtents;

    if ( !pFont )
      pFont = m_pClockFont;

    core::rect<s32> cRect = AbsoluteRect;
		AppBarOption abo = m_appBarOptions[0];
		// move the start X position to the right of the System option
		cRect.UpperLeftCorner.X = abo.rect.LowerRightCorner.X;
		// measure the text
		txtExtents = pFont->getDimension( m_pActiveApp->getAppName() );
		cRect.LowerRightCorner.X = cRect.UpperLeftCorner.X + txtExtents.Width + 30;

		// create the app bar option
		abo.bAppOption = true;
		abo.bHighlight = false;
		abo.rect = cRect;
		abo.str = m_pActiveApp->getAppName();

		m_appBarOptions.push_back(abo);
	}	
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::removeApplicationOptions()
{
	for ( size_t i = 0; i < m_appBarOptions.size(); i++ )
	{
		if ( m_appBarOptions[i].bAppOption )
		{
			m_appBarOptions.erase( m_appBarOptions.begin() + i );
			i--;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::showSystemMenu()
{
	AppBarOption& sysOption = m_appBarOptions[0];
	core::rect<s32> r = sysOption.rect;
	IGUIContextMenu* pMenu = NULL;
	
	r.UpperLeftCorner.Y = r.LowerRightCorner.Y;
	r.LowerRightCorner.X = r.UpperLeftCorner.X + 200;
	r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 300;
	pMenu = Environment->addContextMenu(r, Parent);

	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_SOFTWAREMGR).c_str(), MENUITEM_SOFTWAREMGR);
	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_ITEMMGR).c_str(), MENUITEM_ITEMMGR);
	pMenu->addSeparator();
	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_CHARINFO).c_str(), MENUITEM_CHARINFO);
	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_SYSTEMINFO).c_str(), MENUITEM_SYSTEMINFO);
	pMenu->addSeparator();
	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_ABOUT).c_str(), MENUITEM_ABOUT);
	pMenu->addItem(ResourceManager::instance().GetClientString(STR_APP_APPBAR_SYSTEM_MENU_EXIT).c_str(), MENUITEM_EXIT);
	r.LowerRightCorner.X += 200;
	pMenu->setRelativePosition(r);
	Environment->setFocus(pMenu);
}

//////////////////////////////////////////////////////////////////////////////////////////

void DesktopAppBar::updateAppletLayout()
{
  core::rect<s32> tRect = AbsoluteRect, aRect;
	AppBarOption& abo = m_appBarOptions[0];
  AppBarApplets::iterator it = m_applets.begin();
  AppBarApplets::iterator limit = m_applets.end();
  
  // setyp the rect for the system tray area
  tRect.LowerRightCorner.X = m_rectClock.UpperLeftCorner.X;
  tRect.UpperLeftCorner.X = abo.rect.LowerRightCorner.X+2;
  aRect = tRect;

  for ( ; it != limit; ++it )
  {
    (*it)->getAppletIconRect(aRect);
    tRect.LowerRightCorner.X = aRect.UpperLeftCorner.X;
    aRect = tRect;
  }
}