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
#include "ViewLogicGame.h"

ViewLogicGame::ViewLogicGame(void)
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
{
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
	// temporary 3d code

	// create some 3d object here
	irr::scene::ISceneManager* pSceneMgr = m_pDevice->getSceneManager();

	if ( pSceneMgr )
	{
		irr::scene::IMeshSceneNode* pCube = pSceneMgr->addSphereSceneNode(50, 64);
		if ( pCube )
		{
			pCube->setPosition( core::vector3df(0, 0, 0) );
			pCube->setMaterialFlag( EMF_LIGHTING, false );
			pCube->setMaterialTexture( 0, m_pDevice->getVideoDriver()->getTexture("./clientdata/niyothumb.jpg") );

			irr::scene::ISceneNodeAnimator* pAnim = pSceneMgr->createRotationAnimator( core::vector3df(0, 0.3f, 0) );
			pCube->addAnimator(pAnim);
			pAnim->drop();

		}

    scene::ICameraSceneNode* cam = pSceneMgr->addCameraSceneNode();
    cam->setPosition(core::vector3df(-100,50,100));
    cam->setTarget(core::vector3df(0,0,0));
	}
}

///////////////////////////////////////////////////////////////////////

void ViewLogicGame::Refresh()
{
	m_pScene->drawAll();
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

	return bHandled;
}
