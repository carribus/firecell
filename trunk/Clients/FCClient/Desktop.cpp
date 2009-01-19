#include <sstream>
#include "FCModel.h"
#include "FCView.h"
#include "ViewLogicGame.h"
#include "Desktop.h"

#include "ConsoleWindow.h"

#define ICON_PADDING_X						0
#define ICON_PADDING_Y						30

Desktop::Desktop(ViewLogicGame& owner, IrrlichtDevice* pDevice)
: m_owner(owner)
, m_pDevice(pDevice)
{
	m_iconMax.Height = m_iconMax.Width = 0;
}

///////////////////////////////////////////////////////////////////////

Desktop::~Desktop(void)
{
  if ( m_pBackground )
    m_pBackground->drop();
}

///////////////////////////////////////////////////////////////////////

bool Desktop::Create()
{
  // load the desktop graphic and create a scaled version of it to act as the wallpaper
	core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
	IImage* pImg = m_pDevice->getVideoDriver()->createImageFromFile("./clientdata/desktop.jpg");
	IImage* pImg2 = m_pDevice->getVideoDriver()->createImage(pImg->getColorFormat(), screenDim);

	pImg->copyToScaling( pImg2 );
  pImg->drop();

	m_pBackground = m_pDevice->getVideoDriver()->addTexture("desktop", pImg2);
  pImg2->drop();

	// create the font(s)
	m_pFontCourier = m_pDevice->getGUIEnvironment()->getFont("./clientdata/fonts/fontcourier.png");

  // Get the desktop options available to the player
  UpdateDesktopOptions();

  return true;
}

///////////////////////////////////////////////////////////////////////

void Desktop::Draw()
{
	core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
	m_pDevice->getVideoDriver()->draw2DImage( m_pBackground, 
																					 core::position2d<s32>(0, 0),
																					 core::rect<s32>(0, 0, screenDim.Width, screenDim.Height) );

  DrawDesktopIcons();
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
		case	 DOT_Console:
			{
				// at the moment, we only allow one instance per application type
				if ( !IsApplicationRunning(option.type) )
				{
					ConsoleWindow* pConsole = new ConsoleWindow(m_owner.GetContainer()->GetController(), m_pDevice->getGUIEnvironment());
					if ( pConsole->Create(optionID, L"Console Window") )
					{
						m_arrApps.push_back(pConsole);
					}
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

    default:
      break;
    }
  }
  else
    bResult = false;

  return bResult;
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

	for ( ; it != limit; it++ )
	{
		DesktopOption& d = it->second;
		iconDim = d.pTexture->getSize();
		iconPos.X = pos.X + (m_iconMax.Width/2 - iconDim.Width/2);
		iconPos.Y = pos.Y;
		pVideo->draw2DImage( d.pTexture, 
												 iconPos, 
												 rect<s32>(0, 0, iconDim.Width, iconDim.Height), 
												 0, 
												 SColor(255, 255, 255, 255), 
												 true );

		d.rect.UpperLeftCorner = pos;
		d.rect.LowerRightCorner.X = pos.X + m_iconMax.Width;
		d.rect.LowerRightCorner.Y = pos.Y + m_iconMax.Height;
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