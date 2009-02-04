#include <sstream>
#include "clientstrings.h"
#include "FCModel.h"
#include "FCView.h"
#include "../common/ResourceManager.h"
#include "ViewLogicGame.h"
#include "Desktop.h"

#include "ForumWindow.h"
#include "ConsoleWindow.h"

#define ICON_PADDING_X						0
#define ICON_PADDING_Y						30

#define INGAMEAPP_BASE_ID         0xA000

Desktop::Desktop(ViewLogicGame& owner, IrrlichtDevice* pDevice)
: IGUIElement(EGUIET_ELEMENT, pDevice->getGUIEnvironment(), 0, -1, core::rect<s32>(0, 0, 0, 0))
, m_owner(owner)
, m_pDevice(pDevice)
, m_pAppBar(NULL)
{
  // set the desktop dimensions
  core::dimension2d<s32> screenRes = pDevice->getVideoDriver()->getScreenSize();
  AbsoluteRect = core::rect<s32>(0, 0, screenRes.Width, screenRes.Height);
  AbsoluteClippingRect = AbsoluteRect;

  // load the desktop graphic and scale it as the wallpaper
	IImage* pImg = m_pDevice->getVideoDriver()->createImageFromFile("./clientdata/desktop.jpg");
	IImage* pImg2 = m_pDevice->getVideoDriver()->createImage(pImg->getColorFormat(), screenRes);

	pImg->copyToScaling( pImg2 );
  pImg->drop();

	m_pBackground = m_pDevice->getVideoDriver()->addTexture("desktop", pImg2);
  pImg2->drop();

	// create the font(s)
	m_pFontCourier = m_pDevice->getGUIEnvironment()->getFont("./clientdata/fonts/fontcourier.png");

  // create the desktop app bar
  m_pAppBar = new DesktopAppBar(Environment, this);

  // initialise the max icon height and width threshholds
	m_iconMax.Height = m_iconMax.Width = 0;
}

///////////////////////////////////////////////////////////////////////

Desktop::~Desktop(void)
{
}

///////////////////////////////////////////////////////////////////////

bool Desktop::Create()
{
  // Get the desktop options available to the player
  UpdateDesktopOptions();

  return true;
}

///////////////////////////////////////////////////////////////////////

void Desktop::Draw()
{
  // draw the wallpaper
	core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
	m_pDevice->getVideoDriver()->draw2DImage( m_pBackground, 
																					 core::position2d<s32>(0, 0),
																					 core::rect<s32>(0, 0, screenDim.Width, screenDim.Height) );

  // draw the app bar
  if ( m_pAppBar )
    m_pAppBar->draw();

  // draw the icons
  DrawDesktopIcons();

  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

void Desktop::GetDesktopRect(core::rect<s32>& rect)
{
  rect = AbsoluteRect;
  rect.UpperLeftCorner.Y += m_pAppBar->getAbsolutePosition().getHeight();
}

///////////////////////////////////////////////////////////////////////

bool Desktop::GetDesktopOptionFromPt(s32 x, s32 y, DesktopOption* d)
{
	// check if the point is on any of our desktop icons
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();
	core::rect<s32> iconRect;

	for ( ; it != limit; it++ )
	{
		iconRect = it->second.rect;
		if ( iconRect.isPointInside(position2d<s32>(x, y) ) )
		{
      *d = it->second;
      return true;
		}
	}

  return false;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::GetDesktopOptionByID(FCULONG optionID, DesktopOption& d)
{
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();

	for ( ; it != limit; it++ )
	{
		if ( it->second.optionID == optionID )
		{
			d = it->second;
			return true;
		}
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

void Desktop::HighlightDesktopOption(FCULONG optionID, bool bHighlight)
{
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();

	for ( ; it != limit; it++ )
	{
		if ( it->second.optionID == optionID )
		{
			it->second.isHighlighted = bHighlight;
		}
		else
		{
			if ( bHighlight )
				it->second.isHighlighted = false;
		}
	}

	if ( it != m_mapDesktopOptions.end() )
	{
		it->second.isHighlighted = bHighlight;
	}
}

///////////////////////////////////////////////////////////////////////

void Desktop::ClearAllHighlights()
{
  DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
  DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();

  for ( ; it != limit; it++ )
  {
    it->second.isHighlighted = false;
  }
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OpenApplication(FCULONG optionID, FCSHORT cpuCost, FCULONG memCost)
{
  bool bResult = true;
	DesktopOptionMap::iterator it = m_mapDesktopOptions.find(optionID);

	if ( it != m_mapDesktopOptions.end() )
	{
		DesktopOption option = it->second;
		InGameAppWindow* pAppWnd = NULL;
		
		switch ( option.type )
		{
    case  DOT_Forum:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          ForumWindow* pForum = new ForumWindow(this, m_owner.GetContainer()->GetController(), m_pDevice);

          if ( pForum->Create( INGAMEAPP_BASE_ID+optionID, optionID, ResourceManager::instance().GetClientString( STR_APP_FORUM_CAPTION ) ) )
          {
            m_arrApps.push_back(pForum);
            if ( m_pAppBar )
              m_pAppBar->setActiveApp( pForum );
          }
        }
      }
      break;

    case  DOT_News:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening News!", L"This will be the news browser app window");
        }
      }
      break;

    case  DOT_Email:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Email!", L"This will be the email app window");
        }
      }
      break;

		case	 DOT_Console:
			{
				// at the moment, we only allow one instance per application type
				if ( !IsApplicationRunning(option.type) )
				{
					ConsoleWindow* pConsole = new ConsoleWindow(this, m_owner.GetContainer()->GetController(), m_pDevice);
          if ( pConsole->Create(INGAMEAPP_BASE_ID+optionID, optionID, ResourceManager::instance().GetClientString(STR_APP_CONSOLE_CAPTION) ) )
					{
						m_arrApps.push_back(pConsole);
            if ( m_pAppBar )
              m_pAppBar->setActiveApp( pConsole );
					}
				}				
			}
			break;

    case  DOT_Bank:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Bank!", L"This will be the banking app window");
        }
      }
      break;

    case  DOT_Chat:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Chat!", L"This will be the chatting app window");
        }
      }
      break;

    case  DOT_HackingTools:
      {
        if ( !IsApplicationRunning(option.type) )
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Hacking Toolkit!", L"This will be the hacking toolkit app window");
        }
      }
      break;

		default:
			break;
		}
	}
	else
		bResult = false;

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::IsApplicationRunning(FCUINT appType)
{
	vector<InGameAppWindow*>::iterator it = m_arrApps.begin();
	vector<InGameAppWindow*>::iterator limit = m_arrApps.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetAppType() == appType )
			return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OnGUIEvent(SEvent::SGUIEvent event)
{
	bool bHandled = false;

	switch ( event.EventType )
	{
  case  EGET_ELEMENT_FOCUSED:
    {
      s32 elemID = event.Caller->getID();

      switch ( elemID )
      {
      case  INGAMEAPP_BASE_ID+DOT_Forum:
      case  INGAMEAPP_BASE_ID+DOT_News:
      case  INGAMEAPP_BASE_ID+DOT_Email:
      case  INGAMEAPP_BASE_ID+DOT_Console:
      case  INGAMEAPP_BASE_ID+DOT_Bank:
      case  INGAMEAPP_BASE_ID+DOT_Chat:
      case  INGAMEAPP_BASE_ID+DOT_HackingTools:
        {
          // an application has been activated...
          OnApplicationActivated( this->GetAppWindowByType(elemID-INGAMEAPP_BASE_ID) );
        }
        break;

      default:
        break;
      }
    }
    break;

	case	EGET_ELEMENT_CLOSED:
		{
			IGUIElement* pElem = event.Caller;
			switch ( pElem->getType() )
			{
			case	EGUIET_WINDOW:
				{
					InGameAppWindow* pWnd = NULL;
					AppWindowVector::iterator it = m_arrApps.begin();
					while ( it != m_arrApps.end() )
					{
						if ( (*it)->GetGUIWindow() == pElem )
						{
							pWnd = (*it);
							m_arrApps.erase(it);
              if ( m_pAppBar )
                m_pAppBar->setActiveApp(NULL);
							delete pWnd;
							break;
						}
						it++;
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

///////////////////////////////////////////////////////////////////////

bool Desktop::OnConsoleEvent(FCModelEvent& event)
{
  bool bResult = true;
  ConsoleWindow* pWnd = (ConsoleWindow*)GetAppWindowByType(DOT_Console);

  if ( pWnd )
  {
    switch ( event.GetType() )
    {
    case  FCME_Console_FileSystemInfo:
      {
        FCModel::FileSystemInfo* pFSI = (FCModel::FileSystemInfo*) event.GetData();
        if ( pFSI )
        {
          pWnd->OnFileSystemInfoReceived(pFSI);
        }
      }
      break;

		case	FCME_Console_Command:
			{
				__FCPKT_CON_COMMAND_RESP* pResp = (__FCPKT_CON_COMMAND_RESP*)event.GetData();
				if ( pResp )
				{
          std::string result;
          result.assign( pResp->result, pResp->len );
          pWnd->OnConsoleCommandResponse( pResp->currentDir, result );
				}
			}
			break;

    default:
      break;
    }
  }
  else
    bResult = false;

  return bResult;
}

///////////////////////////////////////////////////////////////////////

void Desktop::OnApplicationActivated(InGameAppWindow* pApp)
{
  if ( !pApp )
    return;

  m_pAppBar->setActiveApp(pApp);
}

///////////////////////////////////////////////////////////////////////

void Desktop::UpdateDesktopOptions()
{
	FCModel* pModel = m_owner.GetContainer()->GetModel();
	std::map<FCUINT, FCModel::DesktopOption> mapOptions = pModel->GetDesktopOptions();
	std::map<FCUINT, FCModel::DesktopOption>::iterator it = mapOptions.begin();
	std::map<FCUINT, FCModel::DesktopOption>::iterator limit = mapOptions.end();
	wstringstream ss;
	std::string iconFilename;

	for ( ; it != limit; it++ )
	{
		DesktopOption d;

		if ( m_mapDesktopOptions.find( it->second.optionID ) == m_mapDesktopOptions.end() )
		{
			d.optionID = it->second.optionID;
			d.type = it->second.type;
			ss << it->second.name;
			d.name = ss.str();

			// clear the stringstream
			ss.str(L"");

			// load the desktop option graphic
			switch ( d.type )
			{
			case	DOT_Forum:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_News:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_Email:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_Console:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_Bank:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_Chat:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;

			case	DOT_HackingTools:
				iconFilename = "./clientdata/gfx/icons/web_normal.png";
				break;
			}
			IImage* pIcon = m_pDevice->getVideoDriver()->createImageFromFile(iconFilename.c_str());
			ITexture* pTexture = m_pDevice->getVideoDriver()->addTexture(iconFilename.c_str(), pIcon);

			core::dimension2d<s32> txtExtents = m_pFontCourier->getDimension( d.name.c_str() );
			d.rect = core::rect<s32>(0, 
															 0, 
															 txtExtents.Width > pTexture->getSize().Width ? txtExtents.Width : pTexture->getSize().Width, 
															 pTexture->getSize().Height + txtExtents.Height + 4);
	
			if ( d.rect.getWidth() > m_iconMax.Width )
				m_iconMax.Width = d.rect.getWidth();
			if ( d.rect.getHeight() > m_iconMax.Height )
				m_iconMax.Height = d.rect.getHeight();

			d.pTexture = pTexture;
			d.isHighlighted = false;
			pIcon->drop();

			// map the desktop option structure
			m_mapDesktopOptions[d.optionID] = d;
		}
	}
}

///////////////////////////////////////////////////////////////////////

void Desktop::DrawDesktopIcons()
{
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();
  IVideoDriver* pVideo = m_pDevice->getVideoDriver();

	core::position2d<s32> pos(10, 10), iconPos, offset;;
	core::dimension2d<s32> iconDim;

  // offset the Y starting position based on the available desktop area
  core::rect<s32> dRect;
  GetDesktopRect(dRect);

  pos.Y += dRect.UpperLeftCorner.Y;

	for ( ; it != limit; it++ )
	{
		DesktopOption& d = it->second;

		iconDim = d.pTexture->getSize();
		iconPos.X = pos.X + (m_iconMax.Width/2 - iconDim.Width/2);
		iconPos.Y = pos.Y;

		d.rect.UpperLeftCorner = pos;
		d.rect.LowerRightCorner.X = pos.X + m_iconMax.Width;
		d.rect.LowerRightCorner.Y = pos.Y + m_iconMax.Height;

		if ( d.isHighlighted )
		{
			pVideo->draw2DImage( d.pTexture, 
													iconPos, 
													rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
													0, 
													SColor(255, 255, 255, 255), 
													true );
		}
		else
		{
			pVideo->draw2DImage( d.pTexture, 
													iconPos, 
													rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
													0, 
													SColor(128, 255, 255, 255), 
													true );
		}


		m_pFontCourier->draw( d.name.c_str(), core::rect<s32>( pos.X, pos.Y + iconDim.Height + 4, d.rect.LowerRightCorner.X, d.rect.LowerRightCorner.Y ), SColor(255, 0, 0, 0), true, true );

		offset.X = 0;
		offset.Y = d.rect.getHeight() + ICON_PADDING_Y;
		
		pos += offset;
	}
}

///////////////////////////////////////////////////////////////////////

InGameAppWindow* Desktop::GetAppWindowByType(FCULONG type)
{
	AppWindowVector::iterator it = m_arrApps.begin();
	AppWindowVector::iterator limit = m_arrApps.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetAppType() == type )
			return (*it);
	}

	return NULL;
}