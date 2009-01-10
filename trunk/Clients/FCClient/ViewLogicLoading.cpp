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
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "ViewLogicLoading.h"

ViewLogicLoading::ViewLogicLoading()
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
, m_pTextObject(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ViewLogicLoading::~ViewLogicLoading(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Create(FCView* pContainer, IrrlichtDevice* pDevice)
{
	m_pContainer = pContainer;

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();
	core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

	ConfigureUISkin();

	// setup the font
	IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontcourier.bmp");

	// create the 'loading' text object
	m_pTextObject = m_pEnv->addStaticText(m_strDetails.c_str(), core::rect<s32>(10, 10, dim.Width, dim.Height), false);

	m_pTextObject->setOverrideColor( SColor(255, 0, 255, 0) );
	m_pTextObject->setOverrideFont(pFont);
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Destroy()
{
	m_pEnv->clear();
	m_pScene->clear();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::SetActive()
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Refresh()
{
	m_pScene->drawAll();
	m_pEnv->drawAll();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::OnModelStateChange(FCModel::StateInfo state)
{
	if ( state.state != FCModel::Loading && state.state != FCModel::Connecting && state.state != FCModel::Login )
    return;

  // check the Model's current sub-state and take necessary action
  switch ( state.state )
  {
  case  FCModel::Loading:
    {
      switch ( state.stateStep )
      {
      case  FCModel::MS_Loading_Text:
				m_strDetails += ResourceManager::instance().GetClientString( STR_LOAD_START );
				m_strDetails += ResourceManager::instance().GetClientString( STR_LOAD_TEXT );
        break;

      case  FCModel::MS_Loading_Graphics:
				m_strDetails += ResourceManager::instance().GetClientString( STR_LOAD_GFX );
        break;

      case  FCModel::MS_Loading_Sounds:
				m_strDetails += ResourceManager::instance().GetClientString( STR_LOAD_AUDIO );
        break;
      }

      m_pTextObject->setText(m_strDetails.c_str());
    }
    break;

  case  FCModel::Connecting:
		{
			switch ( state.stateStep )
			{
			case	FCModel::MS_Connecting_None:
				m_strDetails += ResourceManager::instance().GetClientString( STR_LOAD_SEPERATOR );
				break;

			case	FCModel::MS_Connecting_Connecting:
				m_strDetails += ResourceManager::instance().GetClientString( STR_CONNECT_SERVER );
				break;

			case	FCModel::MS_Connecting_Retry:
				m_strDetails += ResourceManager::instance().GetClientString( STR_CONNECT_FAIL_RETRY );
				break;

			case	FCModel::MS_Connecting_Connected:
				m_strDetails += ResourceManager::instance().GetClientString( STR_CONNECT_OK );
				break;

			case	FCModel::MS_Connecting_FinalFail:
				m_strDetails += ResourceManager::instance().GetClientString( STR_CONNECT_FINAL_FAIL );
				break;
			}

			m_pTextObject->setText(m_strDetails.c_str());
		}
    break;

	case	FCModel::Login:
		{
			s32 x1, y1, x2, y2;

			core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

			x1 = dim.Width / 4;
			y1 = dim.Height / 4;
			x2 = x1 + dim.Width / 2;
			y2 = y2 = dim.Height / 2;
			IGUIWindow* pWindow = m_pEnv->addWindow( core::rect<s32>(x1, y1, x2, y2), true, L"Login to FireCell" );
			pWindow->setVisible(true);
			
		}
		break;

  default:
    break;
  }
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::ConfigureUISkin()
{
	IGUISkin* pSkin = m_pEnv->createSkin(gui::EGST_BURNING_SKIN);

	// Window Background
	pSkin->setColor( EGDC_WINDOW, SColor(255, 0, 0, 0) );
	// Window Caption background
	pSkin->setColor( EGDC_ACTIVE_BORDER, SColor(255, 64, 128, 64) );
	pSkin->setColor( EGDC_ACTIVE_CAPTION, SColor(255, 0, 0, 0) );

	m_pEnv->setSkin(pSkin);
	pSkin->drop();
}