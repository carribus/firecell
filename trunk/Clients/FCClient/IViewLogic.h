#ifndef _IVIEWLOGIC_H_
#define _IVIEWLOGIC_H_

#include <irrlicht.h>

class FCView;

using namespace irr;
using namespace video;

struct IViewLogic
{
	virtual void Create(FCView* pContainer, IrrlichtDevice* pDevice) = 0;
	virtual void Destroy() = 0;
	virtual void SetActive() = 0;
	virtual void Refresh() = 0;
	virtual SColor GetBackgroundColour() = 0;
};

#endif//_IVIEWLOGIC_H_