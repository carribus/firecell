/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <sstream>
#include "../../common/protocol/fcprotocol.h"
#include "FCModel.h"
#include "FCController.h"
#include "FCView.h"
#include "FCViewEvent.h"
#include "ViewLogicGame.h"

#include "ConsoleWindow.h"

#define ICON_PADDING_X						0
#define ICON_PADDING_Y						30

ViewLogicGame::ViewLogicGame(void)
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
, m_pCamera(NULL)
{
	m_iconMax.Height = m_iconMax.Width = 0;
	memset( &m_LButtonLastClick, 0, sizeof(LastClick) );
	memset( &m_RButtonLastClick, 0, sizeof(LastClick) );
}

///////////////////////////////////////////////////////////////////////

ViewLogicGame::~ViewLogicGame(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::Create(FCView* pContainer, IrrlichtDevice* pDevice)
{
	m_pContainer = pContainer;

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();

	// setup the event receiver
	pDevice->setEventReceiver(this);
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::Destroy()
{
	m_pEnv->clear();
	m_pScene->clear();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::SetActive()
{
	// create the font(s)
	m_pFontCourier = m_pEnv->getFont("./clientdata/fonts/fontcourier.png");

  // Create the static camera
  m_pCamera = m_pScene->addCameraSceneNode();
  m_pCamera->setPosition(core::vector3df(0,0,-100));
  m_pCamera->setTarget(core::vector3df(0,0,0));

  // set the ambient lighting
  m_pScene->setAmbientLight(SColorf(1.0f, 1.0f, 1.0f));

  // create the background
  CreateBackgroundPanel();

	// create the 'start bar'
	CreateStartBar();

	// create the Desktop icons
	UpdateDesktopOptions();
/*
	// temporary 3d code

	// create some 3d object here
	irr::scene::ISceneManager* pSceneMgr = m_pDevice->getSceneManager();

	if ( pSceneMgr )
	{
    m_pObject = pSceneMgr->addCubeSceneNode(50);
		if ( m_pObject  )
		{
			m_pObject->setPosition( core::vector3df(0, 0, 0) );
			m_pObject->setMaterialFlag( EMF_LIGHTING, true );
      m_pObject->setMaterialFlag( EMF_ANISOTROPIC_FILTER, true );
			m_pObject->setMaterialTexture( 0, m_pDevice->getVideoDriver()->getTexture("./clientdata/desktop.jpg") );

			irr::scene::ISceneNodeAnimator* pAnim = pSceneMgr->createRotationAnimator( core::vector3df(0, 0.3f, 0.3f) );
			m_pObject->addAnimator(pAnim);
			pAnim->drop();

		}


    // add lighting
    m_pLightNode = pSceneMgr->addLightSceneNode(0, core::vector3df(0, 0, -100), video::SColorf(1.0, 1.0, 1.0));
    m_pLightNode->getLightData().AmbientColor = video::SColorf(0.3, 0.3, 0.3);
    m_pLightNode->getLightData().SpecularColor = video::SColorf(0.5, 0.5, 0.5);
    m_pLightNode->getLightData().Type = video::ELT_DIRECTIONAL;
	}
/*
    //
    irr::scene::ISceneNodeAnimator* pAnim = pSceneMgr->createRotationAnimator( core::vector3df(0, 1.3f, 0) );
    m_pLightNode->addAnimator(pAnim);
    pAnim->drop();
	}
*/
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::Refresh()
{
	// draw the background
	core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
	m_pScene->getVideoDriver()->draw2DImage( m_pBackground, 
																					 core::position2d<s32>(0, 0),
																					 core::rect<s32>(0, 0, screenDim.Width, screenDim.Height) );

	m_pScene->drawAll();

	// draw desktop icons
	DrawDesktopIcons();

	m_pEnv->drawAll();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::OnModelStateChange(FCModel::StateInfo state)
{
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnEvent(const SEvent& event)
{
	bool bHandled = false;

	switch ( event.EventType )
	{
	case	EET_MOUSE_INPUT_EVENT:
		{
			switch ( event.MouseInput.Event )
			{
			case	EMIE_LMOUSE_PRESSED_DOWN:
				bHandled = OnLButtonDown(event.MouseInput);
				break;

			case	EMIE_LMOUSE_LEFT_UP:
				bHandled = OnLButtonUp(event.MouseInput);
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

bool ViewLogicGame::OnLButtonDown(const SEvent::SMouseInput& event)
{
	irr::ITimer* pTimer = m_pDevice->getTimer();
	u32 now = pTimer->getTime();

	if ( m_LButtonLastClick.last_tick )
	{
		if ( (event.X > m_LButtonLastClick.lastX-3 && event.X < m_LButtonLastClick.lastX+3) &&
			   (event.Y > m_LButtonLastClick.lastY-3 && event.Y < m_LButtonLastClick.lastY+3) )
		{
			if ( now - m_LButtonLastClick.last_tick <= 300 )
			{
				memset( &m_LButtonLastClick, 0, sizeof(LastClick) );
				return OnLButtonDblClick(event);
			}
		}
	}

	// update the tracking structure
	m_LButtonLastClick.last_tick = now;
	m_LButtonLastClick.lastX = event.X;
	m_LButtonLastClick.lastY = event.Y;

	return false;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnLButtonUp(const SEvent::SMouseInput& event)
{
	return false;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnLButtonDblClick(const SEvent::SMouseInput& event)
{
	// check if the user double clicked on any of our desktop icons
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();
	core::rect<s32> iconRect;

	for ( ; it != limit; it++ )
	{
		iconRect = it->second.rect;
		if ( iconRect.isPointInside(position2d<s32>(event.X, event.Y) ) )
		{
			// fire an event to the controller
			FCViewEvent e(VE_DesktopOptionActivated, it->second.optionID);
			m_pContainer->GetController()->OnViewEvent(e);
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::CreateBackgroundPanel()
{
	core::dimension2d<s32> screenDim = m_pDevice->getVideoDriver()->getScreenSize();
	IImage* pImg = m_pDevice->getVideoDriver()->createImageFromFile("./clientdata/desktop.jpg");
	IImage* pImg2 = m_pDevice->getVideoDriver()->createImage(pImg->getColorFormat(), screenDim);

	pImg->copyToScaling( pImg2 );

	m_pBackground = m_pDevice->getVideoDriver()->addTexture("desktop", pImg2);
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::CreateStartBar()
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::UpdateDesktopOptions()
{
	FCModel* pModel = m_pContainer->GetModel();
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

void ViewLogicGame::DrawDesktopIcons()
{
	DesktopOptionMap::iterator it = m_mapDesktopOptions.begin();
	DesktopOptionMap::iterator limit = m_mapDesktopOptions.end();

	core::position2d<s32> pos(10, 10), iconPos, offset;;
	core::dimension2d<s32> iconDim;

	for ( ; it != limit; it++ )
	{
		DesktopOption& d = it->second;
		iconDim = d.pTexture->getSize();
		iconPos.X = pos.X + (m_iconMax.Width/2 - iconDim.Width/2);
		iconPos.Y = pos.Y;
		m_pScene->getVideoDriver()->draw2DImage( d.pTexture, 
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

bool ViewLogicGame::OnOpenApplication(FCULONG optionID, FCSHORT cpuCost, FCULONG memCost)
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
					ConsoleWindow* pConsole = new ConsoleWindow(m_pContainer->GetController(), m_pEnv);
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

bool ViewLogicGame::OnConsoleFileSystemInfo(FCModelEvent& event)
{
	bool bResult = true;
	FCModel::FileSystemInfo* pFSI = (FCModel::FileSystemInfo*)event.GetData();
	ConsoleWindow* pWnd = (ConsoleWindow*)GetAppWindowByType(DOT_Console);

	if ( pWnd )
	{
		pWnd->SetCurrentDir( pFSI->currentDir );
		pWnd->SetDirSeperator( pFSI->dirSeperator );
		pWnd->SetFileSystemStyle( pFSI->fsStyle );
	}
	else 
		bResult = false;

	return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::IsApplicationRunning(FCUINT appType)
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

InGameAppWindow* ViewLogicGame::GetAppWindowByType(FCULONG type)
{
	vector<InGameAppWindow*>::iterator it = m_arrApps.begin();
	vector<InGameAppWindow*>::iterator limit = m_arrApps.end();

	for ( ; it != limit; it++ )
	{
		if ( (*it)->GetAppType() == type )
			return (*it);
	}

	return NULL;
}