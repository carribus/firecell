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

#include "IViewLogic.h"

using namespace std;
using namespace irr::scene;
using namespace irr::gui;

class ViewLogicGame :	public IViewLogic
									  , protected IEventReceiver
{
	struct DesktopOption
	{
    FCULONG optionID;
    FCUINT type;
    wstring name;
		ITexture* pTexture;
		core::rect<s32> rect;
	};

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
  void OnModelStateChange(FCModel::StateInfo state);
	SColor GetBackgroundColour()													{ return SColor(255, 0, 0, 0); }

	/*
	 *	IEventReceiver implementation
	 */
	bool OnEvent(const SEvent& event);

private:

  // 3D object creation functions
  void CreateBackgroundPanel();
	void CreateStartBar();	
	void UpdateDesktopOptions();

	// drawing functions
	void DrawDesktopIcons();

	FCView*											m_pContainer;
	IrrlichtDevice*							m_pDevice;
	ISceneManager*							m_pScene;
	IGUIEnvironment*						m_pEnv;

	ITexture*										m_pBackground;

	IGUIFont*										m_pFontCourier;

  ICameraSceneNode*           m_pCamera;
  IMeshSceneNode*             m_pObject;
  ILightSceneNode*            m_pLightNode;

	typedef std::map<FCUINT, DesktopOption> DesktopOptionMap;
	DesktopOptionMap						m_mapDesktopOptions;

	core::dimension2d<s32>			m_iconMax;
};

#endif//_VIEWLOGICGAME_H_