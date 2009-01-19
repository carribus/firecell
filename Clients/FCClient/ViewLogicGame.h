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
#ifndef _VIEWLOGICGAME_H_
#define _VIEWLOGICGAME_H_

#include <irrlicht.h>
#include "IViewLogic.h"
#include "Desktop.h"
#include "FCModelEvent.h"

using namespace std;
using namespace irr;
using namespace scene;
using namespace gui;

class ViewLogicGame :	public IViewLogic
									  , protected IEventReceiver
{
public:

  ViewLogicGame(void);
	~ViewLogicGame(void);

	/*
	 *	IViewLogic implementation
	 */
	void Create(FCView* pContainer, IrrlichtDevice* pDevice);
	void Destroy();
	void SetActive();
	void Refresh();
  bool OnModelEvent(FCModelEvent event);
	SColor GetBackgroundColour()													{ return SColor(255, 0, 0, 0); }
  FCView* GetContainer()                                { return m_pContainer; }

	/*
	 *	IEventReceiver implementation
	 */
	bool OnEvent(const SEvent& event);

	/*
	 *	Public Methods
	 */
	bool OnConsoleFileSystemInfo(FCModelEvent& event);

private:

  void OnModelStateChange(FCModel::StateInfo state);

	/*
	 *	User Input event handlers
	 */
	bool OnLButtonDown(const SEvent::SMouseInput& event);
	bool OnLButtonUp(const SEvent::SMouseInput& event);
	bool OnLButtonDblClick(const SEvent::SMouseInput& event);
	bool OnMouseMove(const SEvent::SMouseInput& event);

  // 3D object creation functions
	void CreateStartBar();	

	FCView*											m_pContainer;
	IrrlichtDevice*							m_pDevice;
	ISceneManager*							m_pScene;
	IGUIEnvironment*						m_pEnv;

  Desktop*                    m_pDesktop;

  ICameraSceneNode*           m_pCamera;
  IMeshSceneNode*             m_pObject;
  ILightSceneNode*            m_pLightNode;

	struct LastClick
	{
		u32 last_tick;
		s32 lastX;
		s32	lastY;
	};
	LastClick										m_LButtonLastClick,
															m_RButtonLastClick;
};

#endif//_VIEWLOGICGAME_H_