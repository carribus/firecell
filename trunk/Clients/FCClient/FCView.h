#ifndef _FCVIEW_H_
#define _FCVIEW_H_

#include <irrlicht.h>
#include "IModelEventSink.h"
#include "FCModel.h"

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

	void OnModelEvent() {}

private:

	FCModel*					m_pModel;
	FCController*			m_pController;

	IrrlichtDevice*		m_pDevice;
	IVideoDriver*			m_pDriver;
	IGUIEnvironment*	m_pEnv;
};

#endif//_FCVIEW_H_