#include "../common/irrlichtUtil/irrutils.h"
#include "FCGUIElementFactory.h"
#include "GUIVUMeter.h"

GUIVUMeter::GUIVUMeter(IGUIEnvironment*& environment, core::rect<s32> rect, IGUIElement*& pParent, SColor backColor, SColor barColor, s32 id)
: IGUIElement((EGUI_ELEMENT_TYPE)FCGUIElementFactory::EGUIET_VUMETER, environment, pParent, id, rect)
, m_backColor(backColor)
, m_barColor(barColor)
, m_RangeMax(100)
, m_currentVal(0)
, m_style(Blocks)
{
	#ifdef _DEBUG
	setDebugName("GUIVUMeter");
	#endif
}

///////////////////////////////////////////////////////////////////////

GUIVUMeter::~GUIVUMeter(void)
{
}

///////////////////////////////////////////////////////////////////////

void GUIVUMeter::draw()
{
  if ( !IsVisible )
    return;

  core::rect<s32> barRect = AbsoluteRect;
  
  // draw the background
  switch ( m_style )
  {
  case  Blocks:
    {
      core::rect<s32> blockRect = AbsoluteRect;

      blockRect.LowerRightCorner.X = blockRect.UpperLeftCorner.X + 3;
      while ( blockRect.UpperLeftCorner.X < AbsoluteRect.LowerRightCorner.X )
      {
        Environment->getVideoDriver()->draw2DRectangle( blockRect, m_backColor, m_backColor, m_backColor, m_backColor );
        offsetRect(blockRect, 5, 0);
      }
    }
    break;

  case  Solid:
  default:
    Environment->getVideoDriver()->draw2DRectangle( AbsoluteRect, m_backColor, m_backColor, m_backColor, m_backColor );
    break;
  }

  // calculate the percentage of the bar to cover
  double dBar = ( ((double)m_currentVal / (double)m_RangeMax) * (double)AbsoluteRect.getWidth() );

  barRect.LowerRightCorner.X = barRect.UpperLeftCorner.X + (s32)dBar;

  // draw the bar
  switch ( m_style )
  {
  case  Blocks:
    {
      core::rect<s32> blockRect = AbsoluteRect;

      blockRect.LowerRightCorner.X = blockRect.UpperLeftCorner.X + 3;
      while ( blockRect.UpperLeftCorner.X < barRect.LowerRightCorner.X )
      {
        Environment->getVideoDriver()->draw2DRectangle( blockRect, m_barColor, m_barColor, m_barColor, m_barColor );
        offsetRect(blockRect, 5, 0);
      }
    }
    break;

  case  Solid:
  default:
    Environment->getVideoDriver()->draw2DRectangle( barRect, m_barColor, m_barColor, m_barColor, m_barColor );
    break;
  }

  // call base drawing code to draw children
  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////

bool GUIVUMeter::OnEvent(const SEvent& event)
{
  return false;
}

///////////////////////////////////////////////////////////////////////

void GUIVUMeter::setRange(u32 max)
{
  m_RangeMax = max;
}

///////////////////////////////////////////////////////////////////////

void GUIVUMeter::setValue(u32 val)
{
  m_currentVal = val;
}

///////////////////////////////////////////////////////////////////////

