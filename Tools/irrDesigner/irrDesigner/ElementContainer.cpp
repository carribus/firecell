#include "StdAfx.h"
#include "ElementContainer.h"

ElementContainer::ElementContainer(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id, rect<s32> r)
: IGUIElement(EGUIET_ELEMENT, pEnv, pParent, id, r)
{
}

ElementContainer::~ElementContainer(void)
{
}

void ElementContainer::draw()
{
}