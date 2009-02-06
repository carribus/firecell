#ifndef _IRRSINGLETON_H_
#define _IRRSINGLETON_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace video;
using namespace gui;

class irrSingleton
{
	irrSingleton(void);
	~irrSingleton(void);

public:

	static irrSingleton& instance();
	static void destroy();

	void setDevice(IrrlichtDevice* pDevice)							{ m_pDevice = pDevice; }
	IrrlichtDevice* getDevice()													{ return m_pDevice; }
	void setTimer(ITimer* pTimer)												{ m_pTimer = pTimer; }
	ITimer* getTimer()																	{ return m_pTimer; }

private:

	static irrSingleton*		m_pThis;
	IrrlichtDevice*					m_pDevice;
	ITimer*									m_pTimer;
};

#endif//_IRRSINGLETON_H_