#ifndef _IRRUTILS_H_
#define _IRRUTILS_H_

#include <irrlicht.h>

namespace irr
{

namespace core
{

/**
  *  @brief Utility function to quickly offset a rectangle's position by a specific XY offset
  */
void offsetRect(core::rect<s32>& rect, s32 xOffs, s32 yOffs);

/**
 *  @brief Convert a rectangle in absolute co-ordinates to relative co-ordinates
 *  @param pParent The element against which the absolute-to-relative coordinates should be calculated
 *  @param rect The rectangle to convert
 */
void absToRelRect(irr::gui::IGUIElement* pParent, core::rect<s32>& rect);

/**
 *  @brief Convert a rectangle in relative co-ordinates to absolute co-ordinates
 *  @param pParent The element against which the relative-to-absolute coordinates should be calculated
 *  @param rect The rectangle to convert
 */
void relToAbsRect(irr::gui::IGUIElement* pParent, core::rect<s32>& rect);

} // end of namespace core

} // end of namespace irr
#endif//_IRRUTILS_H_