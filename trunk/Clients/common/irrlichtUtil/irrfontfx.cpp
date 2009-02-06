#include "irrfontfx.h"

namespace irr
{
namespace gui
{

void drawStrokedFont(IGUIFont* pFont, const wchar_t* text, const SColor& strokeCol, const SColor& textCol, const rect<s32>& rect, bool hcenter, bool vcenter)
{
	core::rect<s32> tRect = rect;
	struct offset_values
	{
		s32 xOff;
		s32 yOff;
	} offsets[] =
	{	{  0, -1 },	{ -1, -1 }, { -1,  0 },	{ -1,  1 },	{  0,  1 },	{  1,  1 },	{  1,  0 },	{  1, -1 } };

	for ( int i = 0; i < 8; i++ )
	{
		tRect = rect;
		tRect.UpperLeftCorner.X += offsets[i].xOff; tRect.UpperLeftCorner.Y += offsets[i].yOff;
		tRect.LowerRightCorner.X += offsets[i].xOff; tRect.UpperLeftCorner.Y += offsets[i].yOff;
		pFont->draw( text, tRect, strokeCol, hcenter, vcenter, &tRect);

	}

	// draw the white text
  pFont->draw( text, rect, textCol, hcenter, vcenter, &rect);
}

}	// end of namespace gui
} // end of namespace irr