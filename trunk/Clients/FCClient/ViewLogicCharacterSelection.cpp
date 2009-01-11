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
#include "ViewLogicCharacterSelection.h"

ViewLogicCharacterSelection::ViewLogicCharacterSelection(void)
: m_pContainer(NULL)
, m_pDevice(NULL)
, m_pScene(NULL)
, m_pEnv(NULL)
{
}

///////////////////////////////////////////////////////////////////////

ViewLogicCharacterSelection::~ViewLogicCharacterSelection(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Create(FCView* pContainer, IrrlichtDevice* pDevice)
{
	m_pContainer = pContainer;

	// get the required interfaces and store them
	m_pDevice = pDevice;
	m_pScene = m_pDevice->getSceneManager();
	m_pEnv = m_pDevice->getGUIEnvironment();

	// setup the event receiver
	pDevice->setEventReceiver(this);

	// setup the font
	IGUIFont* pFont = m_pEnv->getFont("./clientdata/fonts/fontcourier.bmp");
	m_pEnv->getSkin()->setFont(pFont);
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Destroy()
{
	m_pEnv->clear();
	m_pScene->clear();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::SetActive()
{
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::Refresh()
{
	m_pScene->drawAll();
	m_pEnv->drawAll();
}

///////////////////////////////////////////////////////////////////////

void ViewLogicCharacterSelection::OnModelStateChange(FCModel::StateInfo state)
{
}

///////////////////////////////////////////////////////////////////////

bool ViewLogicCharacterSelection::OnEvent(const SEvent& event)
{
	bool bHandled = false;

	return bHandled;
}