#include "irrutils.h"

namespace irr
{

namespace core
{

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void offsetRect(core::rect<s32>& rect, s32 xOffs, s32 yOffs)
{
  rect.UpperLeftCorner.Y += yOffs;
  rect.UpperLeftCorner.X += xOffs;
  rect.LowerRightCorner.Y += yOffs;
  rect.LowerRightCorner.X += xOffs;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void absToRelRect(irr::gui::IGUIElement* pParent, core::rect<s32>& rect)
{
  if ( !pParent )
    return;

  core::rect<s32> aRect = pParent->getAbsolutePosition();
  core::rect<s32> rRect = pParent->getRelativePosition();

  rect.UpperLeftCorner.X += (rRect.UpperLeftCorner.X - aRect.UpperLeftCorner.X);
  rect.LowerRightCorner.X += (rRect.UpperLeftCorner.X - aRect.UpperLeftCorner.X);
  rect.UpperLeftCorner.Y += (rRect.UpperLeftCorner.Y - aRect.UpperLeftCorner.Y);
  rect.LowerRightCorner.Y += (rRect.UpperLeftCorner.Y - aRect.UpperLeftCorner.Y);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void relToAbsRect(irr::gui::IGUIElement* pParent, core::rect<s32>& rect)
{
  if ( !pParent )
    return;

  core::rect<s32> aRect = pParent->getAbsolutePosition();
  core::rect<s32> rRect = pParent->getRelativePosition();

  rect.UpperLeftCorner.X -= (rRect.UpperLeftCorner.X - aRect.UpperLeftCorner.X);
  rect.LowerRightCorner.X -= (rRect.UpperLeftCorner.X - aRect.UpperLeftCorner.X);
  rect.UpperLeftCorner.Y -= (rRect.UpperLeftCorner.Y - aRect.UpperLeftCorner.Y);
  rect.LowerRightCorner.Y -= (rRect.UpperLeftCorner.Y - aRect.UpperLeftCorner.Y);
}

} // end of namespace core

} // end of namespace irr
