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
#ifndef _FCVIEW_H_
#define _FCVIEW_H_

#include <deque>
#include <irrlicht.h>
#include "IModelEventSink.h"
#include "FCModel.h"
#include "QueueReaderThread.h"
#include "ViewLogicLoading.h"
#include "ViewLogicCharacterSelection.h"
#include "ViewLogicGame.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class FCController;
class FCView : public IModelEventSink
{
public:

	FCView(void);
	~FCView(void);

	void SetModel(FCModel* pModel);
	void SetController(FCController* pController);
  FCModel* GetModel()                                       { return m_pModel; }
	FCController* GetController()															{ return m_pController; }

	bool Initialise(E_DRIVER_TYPE driverType = EDT_OPENGL);
	bool Update();

	void OnModelEvent(FCModelEvent event);
		bool OnModelStateChange(FCModelEvent& event);
		bool OnOpenApplication(FCModelEvent& event);
		bool OnConsoleFileSystemInfo(FCModelEvent& event);

private:

	FCModel*					m_pModel;
	FCController*			m_pController;

	IrrlichtDevice*		m_pDevice;
	IVideoDriver*			m_pDriver;
	IGUIEnvironment*	m_pEnv;
	ISceneManager*		m_pScene;

	QueueReaderThread<FCView, FCModelEvent>				m_thrdEventQueue;
	deque<FCModelEvent>														m_modelEventQueue;

	FCModel::e_ModelState		m_currentModelState;

	IViewLogic*				m_pCurrentViewLogic;
	ViewLogicLoading	m_vlLoading;
	ViewLogicCharacterSelection	m_vlCharSelect;
	ViewLogicGame			m_vlGame;
};

#endif//_FCVIEW_H_