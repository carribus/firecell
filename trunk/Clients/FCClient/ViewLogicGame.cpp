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
#include "../common/ResourceManager.h"
#include "../common/irrlichtUtil/irrutils.h"
#include "clientstrings.h"
#include "FCModel.h"
#include "FCController.h"
#include "FCView.h"
#include "FCViewEvent.h"
#include "ViewLogicGame.h"

#ifdef _DEBUG
#include "../common/irrlichtUtil/irrDiag.h"
#endif//_DEBUG

ViewLogicGame::ViewLogicGame(void)
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
, m_pCamera(NULL)
, m_pFontImpactLarge(NULL)
, m_pDesktop(NULL)
{
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

	// create the font(s)
	m_pFontImpactLarge = m_pDevice->getGUIEnvironment()->getFont("./clientdata/fonts/fontimpact_24px.xml");

	m_textAnimator.setTimer(pDevice->getTimer());

  // instantiate the desktop
  if ( !m_pDesktop )
    m_pDesktop = new Desktop(*this, pDevice);
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
  // Create the static camera
  m_pCamera = m_pScene->addCameraSceneNode();
  m_pCamera->setPosition(core::vector3df(0,0,-100));
  m_pCamera->setTarget(core::vector3df(0,0,0));

  // set the ambient lighting
  m_pScene->setAmbientLight(SColorf(1.0f, 1.0f, 1.0f));

  m_pDesktop->Create();

#ifdef _DEBUG
  irrDiag diag;
  std::string dump = diag.dumpElementTree(m_pEnv->getRootGUIElement());
  OutputDebugString( dump.c_str() );
#endif//_DEBUG

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
*/
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::Refresh()
{
	m_textAnimator.refreshObjects();

	// draw the desktop
  m_pDesktop->Draw();
  m_pScene->drawAll();
	m_pEnv->drawAll();

	// render any animated text
	m_textAnimator.renderObjects();
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnModelEvent(FCModelEvent event)
{
  bool bResult = false;

  // if the event wasn't handled, then we'll try and handle it ourselves
  if ( !bResult )
  {
    switch ( event.GetType() )
    {
    case  FCME_ApplicationAdded:
      {
        bResult = m_pDesktop->AddApplication( (ItemSoftware*)event.GetData() );
      }
      break;

    case  FCME_OpenApplication:
      {
  		  __FCPKT_ACTIVATE_SOFTWARE_RESP* pData = (__FCPKT_ACTIVATE_SOFTWARE_RESP*) event.GetData();
        if ( pData->result == ACTIVATERESULT_OK )
        {
          bResult = m_pDesktop->OpenApplication( pData->itemID, pData->cpu_cost, pData->mem_cost );
        }
        else
        {
          std::wstringstream ss;

          m_pDesktop->ShowError(L"Open Application Error!", 
                                L"An error occurred while attempting to open the application!");
        }
      }
      break;

    case  FCME_Console_FileSystemInfo:
      {
        m_pDesktop->OnConsoleEvent(event);
      }
      break;

		case	FCME_Console_Command:
			{
				m_pDesktop->OnConsoleEvent(event);
			}
			break;

		case	FCME_Forum_CategoriesReceived:
			{
				m_pDesktop->OnForumEvent(event);
			}
			break;

    case  FCME_Forum_CategoryThreadsReceived:
      {
        m_pDesktop->OnForumEvent(event);
      }
      break;

    case  FCME_Forum_ThreadContentReceived:
      {
        m_pDesktop->OnForumEvent(event);
      }
      break;

		case	FCME_Mission_MissionAccepted:
			{
				FCULONG missionID = (FCULONG)event.GetData();
				m_pDesktop->OnMissionEvent(event);
				// add an animated text notification..
				std::wstringstream ss;
				core::rect<s32> r = m_pDesktop->getAbsolutePosition();
        core::dimension2d<s32> fontDim = m_pFontImpactLarge->getDimension(L"Wj");
				ss << ResourceManager::instance().GetMissionString(missionID, ResourceManager::MS_Name).c_str();

        r.UpperLeftCorner.Y = (r.getHeight()/2) - (fontDim.Height/2);
        r.LowerRightCorner.Y = r.UpperLeftCorner.Y + fontDim.Height;

				AnimatedText* pText = new AnimatedText( m_pFontImpactLarge, ResourceManager::instance().GetClientString(STR_MISSION_NEWMISSION).c_str(), SColor(255, 0, 255, 0), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);
				offsetRect(r, 0, r.getHeight());
				pText = new AnimatedText( m_pFontImpactLarge, ss.str().c_str(), SColor(255, 255, 255, 255), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);
			}
			break;

		case	FCME_Mission_Completed:
			{
				FCULONG missionID = (FCULONG)event.GetData();
				m_pDesktop->OnMissionEvent(event);

				// add an animated text notification..
				std::wstringstream ss;
        core::dimension2d<s32> fontDim = m_pFontImpactLarge->getDimension(L"Wj");
				core::rect<s32> r = m_pDesktop->getAbsolutePosition();

        r.UpperLeftCorner.Y = (r.getHeight()/2) - (fontDim.Height/2);
        r.LowerRightCorner.Y = r.UpperLeftCorner.Y + fontDim.Height;

        AnimatedText* pText = new AnimatedText( m_pFontImpactLarge, ResourceManager::instance().GetClientString(STR_MISSION_CONGRATULATIONS).c_str(), SColor(255, 255, 255, 255), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);

				offsetRect(r, 0, r.getHeight());

				pText = new AnimatedText( m_pFontImpactLarge, ResourceManager::instance().GetClientString(STR_MISSION_COMPLETE).c_str(), SColor(255, 255, 0, 0), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);

				offsetRect(r, 0, r.getHeight());

				ss << ResourceManager::instance().GetMissionString(missionID, ResourceManager::MS_Name).c_str();
				pText = new AnimatedText( m_pFontImpactLarge, ss.str().c_str(), SColor(255, 255, 0, 0), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);
			}
			break;

    case  FCME_Bank_Connected:
    case  FCME_Bank_AuthNeeded:
    case  FCME_Bank_NoAccountExists:
    case  FCME_Bank_AccountDetailsUpdated:
      {
        m_pDesktop->OnBankEvent(event);
      }
      break;

    case  FCME_Software_SoftwareInstallSuccess:
    case  FCME_Software_SoftwareInstallFail:
    case  FCME_Software_SoftwareUninstallSuccess:
    case  FCME_Software_SoftwareUninstallFail:
    case  FCME_Software_NetworkPortStatusChangeSuccess:
    case  FCME_Software_NetworkPortStatusChangeFail:
      m_pDesktop->OnSoftwareEvent(event);
      break;

		case	FCME_XP_Gained:
			{
				std::wstringstream ss;
				core::rect<s32> r = m_pDesktop->getAbsolutePosition();
        core::dimension2d<s32> fontDim = m_pFontImpactLarge->getDimension(L"Wj");

        r.UpperLeftCorner.Y = (r.getHeight()/2) - (fontDim.Height/2);
        r.LowerRightCorner.Y = r.UpperLeftCorner.Y + fontDim.Height;

				ss << L"XP: +" << (FCULONG)event.GetData();
				AnimatedText* pText = new AnimatedText( m_pFontImpactLarge, ss.str().c_str(), SColor(255, 0, 255, 0), r);
				m_textAnimator.addTextToAnimate(pText, TextAnimator::TAD_VERTICAL, -4, 25, TextAnimator::TAE_FADEOUT, 50, 75);
			}
			break;

    default:
      break;
    }
  }

  return bResult;
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

			case	EMIE_MOUSE_MOVED:
				bHandled = OnMouseMove(event.MouseInput);
				break;
			}
		}
		break;

	case	EET_GUI_EVENT:
		{
			bHandled = m_pDesktop->OnGUIEvent(event.GUIEvent);
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
  bool bResult = false;

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnMouseMove(const SEvent::SMouseInput& event)
{
	bool bResult = false;

	return bResult;
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::CreateStartBar()
{
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicGame::OnConsoleFileSystemInfo(FCModelEvent& event)
{
	bool bResult = true;

  return bResult;
}

