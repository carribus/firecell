#ifndef _FCVIEW_H_
#define _FCVIEW_H_

#include <deque>
#include <irrlicht.h>
#include "IModelEventSink.h"
#include "FCModel.h"
#include "QueueReaderThread.h"
#include "ViewLogicLoading.h"

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

	bool Initialise(E_DRIVER_TYPE driverType = EDT_OPENGL);
	bool Update();

	void OnModelEvent(FCModelEvent event);
	void HandleEvent(FCModelEvent& e);

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
};

#endif//_FCVIEW_H_