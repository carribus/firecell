#ifndef _GUIVUMETER_H_
#define _GUIVUMETER_H_

#include <irrlicht.h>

namespace irr
{

using namespace video;

namespace gui
{

class GUIVUMeter : public IGUIElement
{
public:
  
  enum Style
  {
    Solid,
    Blocks
  };

  GUIVUMeter(IGUIEnvironment*& environment, core::rect<s32> rect, IGUIElement*& pParent = 0, SColor backColor = SColor(255, 0, 0, 0), SColor barColor = SColor(255, 128, 255, 128), s32 id = -1);
  ~GUIVUMeter(void);

  /*
   *  IGUIElement overrides
   */
  void draw();
  bool OnEvent(const SEvent& event);

  /*
   *  PUBLIC Methods
   */
  void setBackColor(SColor col)                       { m_backColor = col; }
  void setBarColor(SColor col)                        { m_barColor = col; }
  void setRange(u32 max);
  void setValue(u32 val);
  void setStyle(Style style)                          { m_style = style; }
  u32 getRange()                                      { return m_RangeMax; }
  u32 getValue()                                      { return m_currentVal; }
  Style getStyle()                                    { return m_style; }

private:

  SColor m_backColor,
         m_barColor;

  u32 m_RangeMax,
      m_currentVal;

  Style m_style;
      
};

} // end of namespace gui

} // end of namespace irr

#endif//_GUIVUMETER_H_