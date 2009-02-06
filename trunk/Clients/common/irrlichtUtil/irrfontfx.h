#ifndef _IRRFONTFX_H_
#define _IRRFONTFX_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

namespace irr
{

namespace gui
{

void drawStrokedFont(IGUIFont* pFont, const wchar_t* text, const SColor& strokeCol, const SColor& textCol, const rect<s32>& rect, bool hcenter = false, bool vcenter = false);

} // end of namespace gui
} // end of namespace irr

#endif//_IRRFONTFX_H_