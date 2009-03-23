#include <sstream>
#include "clientdefs.h"
#include "clientstrings.h"
#include "FCController.h"
#include "FCModel.h"
#include "FCView.h"
#include "FCViewEvent.h"
#include "ForumModel.h"
#include "../common/ResourceManager.h"
#include "ViewLogicGame.h"
#include "Desktop.h"

#include "ForumWindow.h"
#include "ConsoleWindow.h"

#define ICON_PADDING_X						0
#define ICON_PADDING_Y						30

#define INGAMEAPP_BASE_ID         0xA000
#define DESKTOPICON_BASE_ID       0xB000

Desktop::Desktop(ViewLogicGame& owner, IrrlichtDevice* pDevice)
: IGUIElement(EGUIET_ELEMENT, pDevice->getGUIEnvironment(), pDevice->getGUIEnvironment()->getRootGUIElement(), -1, core::rect<s32>(0, 0, 0, 0))
, m_owner(owner)
, m_pDevice(pDevice)
, m_pAppBar(NULL)
, m_pFontCourier(NULL)
, m_mutexApps(true)
, m_pSoftwareMgr(NULL)
{
#ifdef _DEBUG
  setDebugName("Desktop");
#endif//_DEBUG
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
//	m_iconMax.Height = m_iconMax.Width = 0;
}

///////////////////////////////////////////////////////////////////////

Desktop::~Desktop(void)
{
  if ( m_pSoftwareMgr )
    delete m_pSoftwareMgr;
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

  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OnEvent(const SEvent& event)
{
	bool bResult = false;

	switch ( event.EventType )
	{
	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_LMOUSE_PRESSED_DOWN:
				{
          // unselect all desktop icons
					DesktopIconMap::iterator it = m_mapDesktopIcons.begin();
					DesktopIconMap::iterator limit = m_mapDesktopIcons.end();

					for ( ; it != limit; it++ )
					{
						it->second->setSelected(false);
					}
					bResult = true;
				}
				break;

			case	EMIE_MOUSE_MOVED:
				{
          // monitor the mouse and if it goes over the AppBar, give the appBar focus
					rect<s32> abRect = m_pAppBar->getAbsolutePosition();
					if ( abRect.isPointInside( position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
					{
						Environment->setFocus(m_pAppBar);
					}
				}
				break;
			}
		}
		break;

  case  EET_GUI_EVENT:
    {
      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_FOCUSED:
        {
          s32 elemID = event.GUIEvent.Caller->getID();

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
			    IGUIElement* pElem = event.GUIEvent.Caller;
			    switch ( pElem->getType() )
			    {
			    case	EGUIET_WINDOW:
				    {
					    InGameAppWindow* pWnd = NULL;

              m_mutexApps.Lock();
					    AppWindowVector::iterator it = m_arrApps.begin();
					    while ( it != m_arrApps.end() )
					    {
						    if ( (*it)->GetGUIWindow() == pElem )
						    {
                  // get the InGameAppWindow object
							    pWnd = (*it);
                  // remove it from the active apps list
							    m_arrApps.erase(it);
                  // notify the AppBar that the app has closed
                  if ( m_pAppBar )
                    m_pAppBar->setActiveApp(NULL);
                  // delete the InGameAppWindow Object
							    delete pWnd;
                  // set focus back to the desktop
                  Environment->setFocus(this);
							    break;
						    }
						    it++;
					    }
					    m_mutexApps.Unlock();
				    }
				    break;

          case  EGUIET_ELEMENT:
            {
              FCDialog* pDlg = NULL;

              if ( (pElem == (IGUIElement*)GetUtilityWindow(pElem)) )
              {
                CloseUtilityWindow(pElem);
              }
            }
            break;
          }
        }
        break;

			case	EGET_MENU_ITEM_SELECTED:
				{
					IGUIContextMenu* pMenu = (IGUIContextMenu*)event.GUIEvent.Caller;
					s32 selItem = pMenu->getSelectedItem();

					switch ( pMenu->getItemCommandId(selItem) )
					{
          case  MENUITEM_SOFTWAREMGR:   // Software Manager
            {
              OpenSoftwareManagerWindow();
            }
            break;

          case  MENUITEM_ITEMMGR:       // Item Manager
            break;

					case	MENUITEM_CHARINFO:			// Character Info
						{
						}
						break;

					case	MENUITEM_SYSTEMINFO:	  // System Info
						{
						}
						break;

					case	MENUITEM_ABOUT:					// About FireCell
						{
              Environment->addMessageBox( ResourceManager::instance().GetClientString(STR_ABOUT_CAPTION).c_str(),
                                          ResourceManager::instance().GetClientString(STR_ABOUT_TEXT).c_str() );
						}
						break;

					case	MENUITEM_EXIT:					// Quit FireCell
						{
							FCViewEvent e(VE_ClientExit);
							m_owner.GetContainer()->GetController()->OnViewEvent(e);
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
  	}
  }

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void Desktop::GetDesktopRect(core::rect<s32>& rect)
{
  rect = AbsoluteRect;
  rect.UpperLeftCorner.Y += m_pAppBar->getAbsolutePosition().getHeight();
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OpenApplication(FCULONG optionID, FCSHORT cpuCost, FCULONG memCost)
{
  bool bResult = true;
  DesktopIconMap::iterator it = m_mapDesktopIcons.find(optionID);
  InGameAppWindow* pAppWnd = NULL;

  if ( it != m_mapDesktopIcons.end() )
  {
		// at the moment, we only allow one instance per application type
    if ( !IsApplicationRunning( it->second->getType() ) )
    {
      switch ( it->second->getType() )
      {
      case  DOT_Forum:
        {
          ForumWindow* pForum = new ForumWindow(this, m_owner.GetContainer()->GetController(), m_pDevice);

					m_mutexApps.Lock();
          if ( pForum->Create( INGAMEAPP_BASE_ID+optionID, optionID, ResourceManager::instance().GetClientString( STR_APP_FORUM_CAPTION ) ) )
          {
            addChild( pForum->GetGUIWindow() );
            m_arrApps.push_back(pForum);
            if ( m_pAppBar )
              m_pAppBar->setActiveApp( pForum );
          }
					m_mutexApps.Unlock();
        }
        break;

      case  DOT_News:
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening News!", L"This will be the news browser app window");
        }
        break;

      case  DOT_Email:
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Email!", L"This will be the email app window");
        }
        break;

		  case	 DOT_Console:
			  {
					ConsoleWindow* pConsole = new ConsoleWindow(this, m_owner.GetContainer()->GetController(), m_pDevice);

					m_mutexApps.Lock();
          if ( pConsole->Create(INGAMEAPP_BASE_ID+optionID, optionID, ResourceManager::instance().GetClientString(STR_APP_CONSOLE_CAPTION) ) )
					{
            addChild( pConsole->GetGUIWindow() );
						m_arrApps.push_back(pConsole);
            if ( m_pAppBar )
              m_pAppBar->setActiveApp( pConsole );
					}
					m_mutexApps.Unlock();
			  }
			  break;

      case  DOT_Bank:
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Bank!", L"This will be the banking app window");
        }
        break;

      case  DOT_Chat:
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Chat!", L"This will be the chatting app window");
        }
        break;

      case  DOT_HackingTools:
        {
          m_pDevice->getGUIEnvironment()->addMessageBox(L"Opening Hacking Toolkit!", L"This will be the hacking toolkit app window");
        }
        break;

		  default:
			  break;

      }
    }
		else
		{
			// The application is already running, bring the window to the top
			InGameAppWindow* pWnd = GetAppWindowByType( it->second->getType() );

			if ( pWnd )
			{
				m_pAppBar->setActiveApp(pWnd);
				bringToFront( pWnd->GetGUIWindow() );
			}
		}
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::IsApplicationRunning(FCUINT appType)
{
	m_mutexApps.Lock();
	vector<InGameAppWindow*>::iterator it = m_arrApps.begin();
	vector<InGameAppWindow*>::iterator limit = m_arrApps.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetAppType() == appType )
		{
			m_mutexApps.Unlock();
			return true;
		}
	}

	m_mutexApps.Unlock();

	return false;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OnGUIEvent(SEvent::SGUIEvent event)
{
	bool bHandled = false;

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

bool Desktop::OnForumEvent(FCModelEvent& event)
{
	bool bResult = true;
	ForumWindow* pWnd = (ForumWindow*) GetAppWindowByType( DOT_Forum );

	if ( pWnd )
	{
		switch ( event.GetType() )
		{
		case	FCME_Forum_CategoriesReceived:
			bResult = pWnd->OnCategoriesReceived((ForumModel*)event.GetData());
			break;

    case  FCME_Forum_CategoryThreadsReceived:
      bResult = pWnd->OnCategoryThreadsReceived((FCULONG)event.GetData());
      break;

    case  FCME_Forum_ThreadContentReceived:
      bResult = pWnd->OnThreadContentReceived((ForumThread*)event.GetData());
      break;

		default:
			break;
		}
	}

	return bResult;
}

///////////////////////////////////////////////////////////////////////

bool Desktop::OnMissionEvent(FCModelEvent& event)
{
	bool bResult = false;

	switch ( event.GetType() )
	{
	case	FCME_Mission_MissionAccepted:
		break;

	case	FCME_Mission_Completed:
		{
			FCULONG missionID = (FCULONG)event.GetData();
		}
		break;

	default:
		break;
	}

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void Desktop::OnDesktopIconSelected(DesktopIcon* pIcon)
{
	DesktopIconMap::iterator it = m_mapDesktopIcons.begin();
	DesktopIconMap::iterator limit = m_mapDesktopIcons.end();

	for ( ; it != limit; it++ )
	{
		if ( it->second != pIcon )
			it->second->setSelected(false);
	}

	Environment->setFocus(this);
}

///////////////////////////////////////////////////////////////////////

void Desktop::OnDesktopIconActivated(DesktopIcon* pIcon)
{
	// fire an event to the controller
	FCViewEvent e(VE_DesktopOptionActivated, pIcon->getID()-DESKTOPICON_BASE_ID);
	m_owner.GetContainer()->GetController()->OnViewEvent(e);
}

///////////////////////////////////////////////////////////////////////

void Desktop::OnApplicationActivated(InGameAppWindow* pApp)
{
  if ( !pApp )
    return;

  m_pAppBar->setActiveApp(pApp);
	bringToFront( pApp->GetGUIWindow() );
}

///////////////////////////////////////////////////////////////////////

void Desktop::UpdateDesktopOptions()
{
	FCModel* pModel = m_owner.GetContainer()->GetModel();
	std::map<FCUINT, FCModel::DesktopOption> mapOptions = pModel->GetDesktopOptions();
	std::map<FCUINT, FCModel::DesktopOption>::iterator it = mapOptions.begin();
	std::map<FCUINT, FCModel::DesktopOption>::iterator limit = mapOptions.end();
  core::dimension2d<s32> iconMax;
  core::rect<s32> iRect;
  wstringstream ss;
	std::string iconFilename;
  DesktopIcon* pIcon = NULL;

	for ( ; it != limit; it++ )
  {
    // get the text into a wchar_t format
  	ss << it->second.name;
    // create the desktop icon element
    pIcon = new DesktopIcon(this, Environment, this, ss.str().c_str(), it->second.optionID+DESKTOPICON_BASE_ID);
    ss.str(L"");
    if ( pIcon )
    {
      // set the application type that this icon represents
      pIcon->setType( it->second.type );
      // set the font
      pIcon->setFont(m_pFontCourier);
			// determine which graphic file the icon should be using
			switch ( it->second.type )
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
      pIcon->setIcon(iconFilename);
      iRect = pIcon->getAbsolutePosition();

			if ( iRect.getWidth() > iconMax.Width )
				iconMax.Width = iRect.getWidth();
			if ( iRect.getHeight() > iconMax.Height )
				iconMax.Height = iRect.getHeight();

      m_mapDesktopIcons[ it->second.optionID ] = pIcon;
    }
	}

  // make all icons the same size
  for ( DesktopIconMap::iterator it2 = m_mapDesktopIcons.begin(); it2 != m_mapDesktopIcons.end(); it2++ )
  {
    pIcon = it2->second;
    pIcon->setWidth( iconMax.Width );
    pIcon->setHeight( iconMax.Height );
  }

  updateIconPositions();
}

///////////////////////////////////////////////////////////////////////

void Desktop::updateIconPositions()
{
  DesktopIconMap::iterator it = m_mapDesktopIcons.begin();
  DesktopIconMap::iterator limit = m_mapDesktopIcons.end();
  DesktopIcon* pIcon = NULL;
	core::position2d<s32> pos(10, 10), iconPos, offset;

  // offset the Y starting position based on the available desktop area
  core::rect<s32> dRect;
  GetDesktopRect(dRect);
  pos.Y += dRect.UpperLeftCorner.Y;

  for ( ; it != limit; it++ )
  {
    pIcon = it->second;
    pIcon->moveTo( pos.X, pos.Y );

		offset.X = 0;
		offset.Y = pIcon->getAbsolutePosition().getHeight() + ICON_PADDING_Y;
		
		pos += offset;
  }
}

///////////////////////////////////////////////////////////////////////

InGameAppWindow* Desktop::GetAppWindowByType(FCULONG type)
{
	m_mutexApps.Lock();

	AppWindowVector::iterator it = m_arrApps.begin();
	AppWindowVector::iterator limit = m_arrApps.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetAppType() == type )
		{
			m_mutexApps.Unlock();
			return (*it);
		}
	}

	m_mutexApps.Unlock();

	return NULL;
}

///////////////////////////////////////////////////////////////////////

FCDialog* Desktop::GetUtilityWindow(IGUIElement* pWnd)
{
  if ( !pWnd )
    return NULL;

  FCDialog* windows[] = { m_pSoftwareMgr };
  int limit = sizeof(windows) / sizeof(FCDialog*);

  for ( int i = 0; i < limit; i++ )
  {
    if ( windows[i] )
    {
      if ( windows[i] == pWnd )
        return windows[i];
    }
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////

void Desktop::CloseUtilityWindow(IGUIElement* pWnd)
{
  // this is a trick that allows me to set the class pointer of the utility to window using a generic for loop on the window[] array
  FCDialog** windows[] = { (FCDialog**)&m_pSoftwareMgr };
  int limit = sizeof(windows) / sizeof(FCDialog*);

  for ( int i = 0; i < limit; i++ )
  {
    if ( *windows[i] && *windows[i] == pWnd )
    {
      *windows[i] = NULL;
    }
  }
}

///////////////////////////////////////////////////////////////////////

void Desktop::OpenSoftwareManagerWindow()
{
  if ( !m_pSoftwareMgr )
  {
    if ( (m_pSoftwareMgr = new SoftwareMgrWindow( this, FCModel::instance(), m_pDevice->getGUIEnvironment(), (wchar_t*)ResourceManager::instance().GetClientString( STR_APP_APPBAR_SYSTEM_MENU_SOFTWAREMGR ).c_str(), this, MENUITEM_SOFTWAREMGR)) )
    {
      addChild( m_pSoftwareMgr );
    }
  }
  else
  {
		bringToFront( m_pSoftwareMgr );
 }
}