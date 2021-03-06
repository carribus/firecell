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
#ifndef _VIEWLOGICLOADING_H_
#define _VIEWLOGICLOADING_H_

#include <string>
#include <vector>
#include "../../common/PThreadMutex.h"
#include "IViewLogic.h"

using namespace std;
using namespace irr::scene;
using namespace irr::gui;

class ViewLogicLoading : public IViewLogic
											 , protected IEventReceiver
{
public:
	ViewLogicLoading();
	~ViewLogicLoading(void);

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
    bool OnButtonLoginClicked();

	/*
	 *	Methods
	 */

private:

	void ConfigureUISkin();
  void SetLoginWindowStrings(IGUIWindow* pWindow);

  bool OnModelStateChange(FCModel::StateInfo state);

	FCView*											m_pContainer;
	IrrlichtDevice*							m_pDevice;
	ISceneManager*							m_pScene;
	IGUIEnvironment*						m_pEnv;

	IGUIStaticText*							m_pTextObject;

	std::wstring						    m_strDetails;

  PThreadMutex                m_mutexUpdates;
};

#endif//_VIEWLOGICLOADING_H_