#ifndef _DESKTOPICON_H_
#define _DESKTOPICON_H_

#include <string>
#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace gui;
using namespace video;

class DesktopIcon : public IGUIElement
{
public:
  DesktopIcon(IGUIEnvironment* env, IGUIElement* pParent = 0, const wchar_t* text = 0, s32 id = -1);
  ~DesktopIcon(void);

  void setIcon(const std::string& filename);
  void setText(const wchar_t* text);
  void setFont(IGUIFont* pFont)                                 { m_pFont = pFont; }
  void setWidth(s32 width);
  void setHeight(s32 height);
  void moveTo(s32 x, s32 y);

  rect<s32> calculateDimensions();

  void draw();
  bool OnEvent(const SEvent& event);

private:

  ITexture*               m_pTexture;
  IGUIFont*               m_pFont;
  bool                    m_bSelected;
};

#endif//_DESKTOPICON_H_