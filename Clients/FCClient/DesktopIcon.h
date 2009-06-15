#ifndef _DESKTOPICON_H_
#define _DESKTOPICON_H_

#include <string>
#include <irrlicht.h>
#include "../../common/game_objects/ItemSoftware.h"
#include "IDesktop.h"

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

class DesktopIcon : public IGUIElement
{
public:
  DesktopIcon(IDesktop* pDesktop, IGUIEnvironment* env, IGUIElement* pParent = 0, const wchar_t* text = 0, s32 id = -1);
  ~DesktopIcon(void);

  void setIcon(const std::string& filename);
  void setText(const wchar_t* text);
  void setFont(IGUIFont* pFont)                                 { m_pFont = pFont; }
  void setWidth(s32 width);
  void setHeight(s32 height);
	void setSelected(bool bSelected)															{ m_bSelected = bSelected; }
  void setType(unsigned int type)                               { m_type = type; }
  void setSoftware(ItemSoftware* pSoftware)                     { m_pSoftware = pSoftware; }
  unsigned int getType()                                        { return m_type; }
  void moveTo(s32 x, s32 y);

  ItemSoftware* getSoftware()                                   { return m_pSoftware; }

  rect<s32> calculateDimensions();

  void draw();
  bool OnEvent(const SEvent& event);

  bool OnLButtonDown(const SEvent::SMouseInput& event);
  bool OnLButtonDblClick(const SEvent::SMouseInput& event);

private:

	IDesktop*								m_pDesktop;
  ITexture*               m_pTexture;
  IGUIFont*               m_pFont;
  bool                    m_bSelected;
  unsigned int            m_type;
  ItemSoftware*           m_pSoftware;

  /*
   *  Double click tracking
   */
	struct LastClick
	{
		u32 last_tick;
		s32 lastX;
		s32	lastY;
	};
	LastClick										m_LButtonLastClick;
};

#endif//_DESKTOPICON_H_