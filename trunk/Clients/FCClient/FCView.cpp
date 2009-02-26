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
#include "../common/irrlichtUtil/irrfontfx.h"
#include "Settings.h"
#include "irrSingleton.h"
#include "FCController.h"
#include "FCGUIElementFactory.h"
#include "FCView.h"

FCView::FCView(void)
: m_pModel(NULL)
, m_pController(NULL)
, m_pDevice(NULL)
, m_pDriver(NULL)
, m_pEnv(NULL)
, m_pScene(NULL)
, m_currentModelState(FCModel::None)
, m_pCurrentViewLogic(NULL)
{
}

///////////////////////////////////////////////////////////////////////

FCView::~FCView(void)
{
	if ( m_pCurrentViewLogic )
		m_pCurrentViewLogic->Destroy();

	if ( m_pDevice )
		m_pDevice->drop();

	irrSingleton::destroy();
}

///////////////////////////////////////////////////////////////////////

void FCView::SetModel(FCModel* pModel)
{
	m_pModel = pModel;
	pModel->SubscribeToEvents(this);
}

///////////////////////////////////////////////////////////////////////

void FCView::SetController(FCController* pController)
{
	m_pController = pController;
}

///////////////////////////////////////////////////////////////////////

bool FCView::Initialise(E_DRIVER_TYPE driverType)
{
	IrrlichtDevice* pDev = createDevice(EDT_NULL);
	IVideoDriver* pDriver = NULL;
	s32 vDepth = 16;
	dimension2d<s32> vRes(800, 600), uRes;
  bool bFullScreen = true;

  uRes.Width = atoi( Settings::instance().GetValue("FCClient/Settings/Resolution", "width").c_str() );
  uRes.Height = atoi( Settings::instance().GetValue("FCClient/Settings/Resolution", "height").c_str() );
  vDepth = atoi( Settings::instance().GetValue("FCClient/Settings/Resolution", "depth").c_str() );
  bFullScreen = (atoi( Settings::instance().GetValue("FCClient/Settings/Fullscreen", "useFullScreen").c_str() ) != 0);

	// get the current desktop resolution and bit depth from the NULL device
  if ( uRes.Width == 0 || uRes.Height == 0 || vDepth == 0 )
  {
	  if ( pDev )
	  {
		  IVideoModeList* pModeList = pDev->getVideoModeList();

		  if ( pModeList )
		  {
			  vDepth = pModeList->getDesktopDepth();
			  vRes = pModeList->getDesktopResolution();
		  }
	  }
  }

	// release the NULL device since we no longer need it
	pDev->closeDevice();
	pDev->drop();

	// now create the real device
	m_pDevice = createDevice(driverType, vRes, vDepth, bFullScreen);
	if ( !m_pDevice )
		return false;

	m_pDriver = m_pDevice->getVideoDriver();
	if ( !m_pDriver )
		return false;

	m_pEnv = m_pDevice->getGUIEnvironment();
	if ( !m_pEnv )
		return false;

	m_pScene = m_pDevice->getSceneManager();
	if ( !m_pScene )
		return false;

  // register the custom gui element factory
  IGUIElementFactory* pFactory = new FCGUIElementFactory(m_pEnv);
  m_pEnv->registerGUIElementFactory( pFactory );
  pFactory->drop();

	// create the singleton object for irrlicht object access throughout the system
	irrSingleton::instance().setDevice( m_pDevice );
	irrSingleton::instance().setTimer( m_pDevice->getTimer() );

	return true;
}

///////////////////////////////////////////////////////////////////////

bool FCView::Update()
{
	bool bResult = true;
	SColor colBkg = SColor(255, 0, 0, 0);

	if ( m_pDevice->run() && m_pDriver )
	{
		if ( m_pCurrentViewLogic )
			colBkg = m_pCurrentViewLogic->GetBackgroundColour();
		m_pDriver->beginScene(true, true, colBkg);
		if ( m_pCurrentViewLogic )
			m_pCurrentViewLogic->Refresh();

#ifdef _DEBUG
	
		// draw the FPS
		std::wstringstream ss;
		dimension2d<s32> screenSize = m_pDriver->getScreenSize();
		ss << L"FPS: " << m_pDriver->getFPS();
		drawStrokedFont( m_pDevice->getGUIEnvironment()->getSkin()->getFont(),
										 ss.str().c_str(), 
										 SColor(255, 0, 0, 0),
										 SColor(255, 0, 255, 0),
										 rect<s32>(0, screenSize.Height-16, screenSize.Width, screenSize.Height), 
										 false, 
										 false );

#endif//_DEBUG

    drawLatency();

		m_pDriver->endScene();
	}
	else
		bResult = false;

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void FCView::OnModelEvent(FCModelEvent event)
{
  bool bHandled = false;

  // attempt to hand the event to the active view (if any)
  if ( m_pCurrentViewLogic )
    bHandled = m_pCurrentViewLogic->OnModelEvent(event);

  // if the event was not handled, then we try and handle it in this object (if possible)
  if ( !bHandled )
  {
    switch ( event.GetType() )
    {
    case  FCME_StateChange:
      OnModelStateChange(event);
      break;

    default:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////

bool FCView::OnModelStateChange(FCModelEvent& event)
{
	FCModel::e_ModelState newState = static_cast<FCModel::e_ModelState>(event.GetData());
	FCModel::StateInfo stateInfo = m_pModel->GetState();

	if ( newState != m_currentModelState )
	{
		IViewLogic* pNewView = NULL;

		switch ( newState )
		{
		case	FCModel::Loading:
			pNewView = &m_vlLoading;
			break;

		case  FCModel::Connecting:
			break;

		case	FCModel::Login:
			break;

		case	FCModel::CharacterSelection:
			pNewView = &m_vlCharSelect;
			break;

		case	FCModel::Playing:
			pNewView = &m_vlGame;
			break;

		case	FCModel::ShuttingDown:
			break;
		}

		if ( pNewView )
		{
			if ( m_pCurrentViewLogic )
      {
				m_pCurrentViewLogic->Destroy();
        m_pCurrentViewLogic = NULL;
      }
			pNewView->Create(this, m_pDevice);
			m_pCurrentViewLogic = pNewView;
			m_pCurrentViewLogic->SetActive();
		}
		m_currentModelState = newState;
	}
	else
	{
		// if the main state did not change, then perhaps the sub-state did
		FCModel::StateInfo stateInfo = m_pModel->GetState();

		if ( m_pCurrentViewLogic )
		{
			switch ( stateInfo.state )
			{
			case  FCModel::Loading:
			case  FCModel::Connecting:
				break;

			case  FCModel::Login:
				break;

			case	FCModel::CharacterSelection:
				break;

			case  FCModel::Playing:
				break;

			case  FCModel::ShuttingDown:
				break;
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

void FCView::drawLatency()
{
  FCULONG latency = FCModel::instance().GetLatency();

  if ( m_pEnv )
  {
    IGUIFont* pFont = m_pEnv->getSkin()->getFont();

    if ( pFont )
    {
		  std::wstringstream ss;
		  dimension2d<s32> screenSize = m_pDriver->getScreenSize();
		  ss << L"Latency: " << latency << L"ms";
		  drawStrokedFont( pFont,
										  ss.str().c_str(), 
										  SColor(255, 0, 0, 0),
										  SColor(255, 0, 255, 0),
										  rect<s32>(screenSize.Width-pFont->getDimension(ss.str().c_str()).Width-10, screenSize.Height-16, screenSize.Width, screenSize.Height), 
										  true, 
										  true);
    }
  }
}