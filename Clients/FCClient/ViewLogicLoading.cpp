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
#include "FCView.h"
#include "FCViewEvent.h"
#include "FCController.h"
#include "ViewLogicLoading.h"

#define WINDOW_LOGIN									1
#define EDIT_USERNAME									2
#define EDIT_PASSWORD									3
#define BUTTON_LOGIN									4
#define BUTTON_CANCEL									5
#define STATIC_USERNAME               6
#define STATIC_PASSWORD               7

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

	// setup the event receiver
	pDevice->setEventReceiver(this);
//	ConfigureUISkin();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Destroy()
{
  m_mutexUpdates.Lock();
	m_pEnv->clear();
	m_pScene->clear();
  m_mutexUpdates.Unlock();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::SetActive()
{
	core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

  m_mutexUpdates.Lock();

	// setup the font
	IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontcourier.png");
	m_pEnv->getSkin()->setFont(pFont);

	// create the 'loading' text object
	m_pTextObject = m_pEnv->addStaticText(m_strDetails.c_str(), core::rect<s32>(10, 10, dim.Width, dim.Height), false);
		
	m_pTextObject->setOverrideColor( SColor(255, 0, 255, 0) );
	m_pTextObject->setOverrideFont(pFont);

  m_mutexUpdates.Unlock();

}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::Refresh()
{
  m_mutexUpdates.Lock();
	m_pScene->drawAll();
	m_pEnv->drawAll();
  m_mutexUpdates.Unlock();
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicLoading::OnModelEvent(FCModelEvent event)
{
  bool bResult = false;

  switch ( event.GetType() )
  {
  case  FCME_StateChange:
    {
    	FCModel::StateInfo stateInfo = m_pContainer->GetModel()->GetState();

      bResult = OnModelStateChange(stateInfo);
    }
    break;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicLoading::OnEvent(const SEvent& event)
{
	bool bHandled = false;

	if ( event.EventType == EET_GUI_EVENT )
	{
		s32 elemID = event.GUIEvent.Caller->getID();
		
		switch ( event.GUIEvent.EventType )
		{
		case	EGET_BUTTON_CLICKED:
			{
				switch ( elemID )
				{
				case	BUTTON_LOGIN:
					{
						wstring username, password;

						IGUIWindow* pWnd = (IGUIWindow*)m_pEnv->getRootGUIElement()->getElementFromId( WINDOW_LOGIN );

						if ( pWnd )
						{
							IGUIEditBox* pEdit = (IGUIEditBox*) pWnd->getElementFromId( EDIT_USERNAME );
							username = pEdit->getText();
							pEdit = (IGUIEditBox*) pWnd->getElementFromId( EDIT_PASSWORD );
							password = pEdit->getText();

              event.GUIEvent.Caller->setEnabled(false);

							FCViewEventLogin e( username, password );
							m_pContainer->GetController()->OnViewEvent(e);
						}
					}
					break;

				case	BUTTON_CANCEL:
					{
						FCViewEvent e( VE_ClientExit );
						m_pContainer->GetController()->OnViewEvent(e);
					}
					break;
				}
			}
			break;

		case	EGET_ELEMENT_CLOSED:
			{
			}
			break;
		}
	}

	return bHandled;
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

	pSkin->setColor( EGDC_BUTTON_TEXT, SColor(255, 64, 255, 64) );

	pSkin->setColor( EGDC_3D_FACE, SColor(255, 196, 255, 196) );


	m_pEnv->setSkin(pSkin);
	pSkin->drop();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicLoading::SetLoginWindowStrings(IGUIWindow* pWindow)
{
  IGUIElement* pElem = NULL;

  pWindow->setText( ResourceManager::instance().GetClientString( STR_LOGINWND_CAPTION ).c_str() );
  if ( (pElem = pWindow->getElementFromId( STATIC_USERNAME )) )
    pElem->setText( ResourceManager::instance().GetClientString( STR_LOGINWND_USERNAME ).c_str() );
  if ( (pElem = pWindow->getElementFromId( STATIC_PASSWORD )) )
    pElem->setText( ResourceManager::instance().GetClientString( STR_LOGINWND_PASSWORD ).c_str() );
  if ( (pElem = pWindow->getElementFromId( BUTTON_LOGIN )) )
    pElem->setText( ResourceManager::instance().GetClientString( STR_LOGINWND_LOGINBUTTON ).c_str() );
  if ( (pElem = pWindow->getElementFromId( BUTTON_CANCEL )) )
    pElem->setText( ResourceManager::instance().GetClientString( STR_LOGINWND_EXITBUTTON ).c_str() );
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicLoading::OnModelStateChange(FCModel::StateInfo state)
{
	if ( state.state != FCModel::Loading && state.state != FCModel::Connecting && state.state != FCModel::Login )
    return false;

  m_mutexUpdates.Lock();

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
			core::dimension2d<s32> dim = m_pDevice->getVideoDriver()->getScreenSize();

      if ( state.stateStep == FCModel::MS_Login_None )
      {
  			m_pEnv->loadGUI("./clientdata/ui/dialog.login.xml");
  			IGUIElement* pRootElem = m_pEnv->getRootGUIElement();
			  IGUIWindow* pWindow = (IGUIWindow*)pRootElem->getElementFromId( WINDOW_LOGIN );

			  if ( pWindow )
			  {
          SetLoginWindowStrings(pWindow);
				  core::rect<s32> wndRect = pWindow->getAbsoluteClippingRect();
				  pWindow->setRelativePosition(core::position2di( dim.Width/2 - wndRect.getWidth() / 2, dim.Height/2 - wndRect.getHeight()/2 ));
				  m_pEnv->setFocus( pWindow->getElementFromId( EDIT_USERNAME ) );
			  }
      }
		}
		break;

  default:
    break;
  }
  m_mutexUpdates.Unlock();

  return true;
}

