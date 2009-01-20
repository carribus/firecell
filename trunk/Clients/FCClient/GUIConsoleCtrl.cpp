#include "FCGUIElementFactory.h"
#include "GuiConsoleCtrl.h"

GUIConsoleCtrl::GUIConsoleCtrl(IGUIEnvironment* environment, core::rect<s32>& rect, IGUIElement* pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement((EGUI_ELEMENT_TYPE)FCGUIElementFactory::EGUIET_CONSOLE, environment, pParent, id, rect)
, m_backColor(backColor)
, m_textColor(textColor)
{
	#ifdef _DEBUG
	setDebugName("GUIConsoleCtrl");
	#endif
}

///////////////////////////////////////////////////////////////////////

GUIConsoleCtrl::~GUIConsoleCtrl(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIConsoleCtrl::draw()
{
  // draw the background
  Environment->getVideoDriver()->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );
  
  // call base drawing code to draw children
  IGUIElement::draw();
}