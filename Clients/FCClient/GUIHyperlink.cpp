#include "FCGUIElementFactory.h"
#include "GUIHyperlink.h"

GUIHyperlink::GUIHyperlink(IGUIEnvironment*& env, core::rect<s32> rect, IGUIElement*& pParent, SColor backColor, SColor textColor, s32 id)
: IGUIElement((EGUI_ELEMENT_TYPE)FCGUIElementFactory::EGUIET_HYPERLINK, env, pParent, id, rect)
, m_pEnv(env)
, m_backColor(backColor)
, m_textColor(textColor)
{
	#ifdef _DEBUG
	setDebugName("GUIHyperlink");
	#endif
}

///////////////////////////////////////////////////////////////////////////////////////////

GUIHyperlink::~GUIHyperlink(void)
{
}

///////////////////////////////////////////////////////////////////////////////////////////

void GUIHyperlink::draw()
{
  if ( !IsVisible )
    return;

  IGUIFont* pFont = m_pEnv->getSkin()->getFont();
  IVideoDriver* pVideo = m_pEnv->getVideoDriver();
  core::dimension2d<s32> fontDim = pFont->getDimension(getText());

  pFont->draw( getText(), AbsoluteRect, m_textColor, false, false, &AbsoluteClippingRect );
  if ( AbsoluteRect.UpperLeftCorner.Y + fontDim.Height <= AbsoluteRect.LowerRightCorner.Y )
  {
    if ( fontDim.Width > AbsoluteClippingRect.getWidth() )
      fontDim.Width = AbsoluteClippingRect.getWidth();

    pVideo->draw2DLine( core::position2d<s32>( AbsoluteRect.UpperLeftCorner.X, AbsoluteRect.UpperLeftCorner.Y + fontDim.Height ),
                        core::position2d<s32>( AbsoluteRect.UpperLeftCorner.X + fontDim.Width, AbsoluteRect.UpperLeftCorner.Y + fontDim.Height ),
                        m_textColor );
  }

  IGUIElement::draw();
}

///////////////////////////////////////////////////////////////////////////////////////////

bool GUIHyperlink::OnEvent(const SEvent& event)
{
  switch ( event.EventType )
  {
  case  EET_GUI_EVENT:
    {
      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_HOVERED:
        {
        }
        break;

      case  EGET_ELEMENT_LEFT:
        break;

      default:
        break;
      }
    }
    break;

  case  EET_MOUSE_INPUT_EVENT:
    {
      switch (event.MouseInput.Event)
      {
      case  EMIE_LMOUSE_LEFT_UP:
        {
          if ( isPointInside( core::position2d<s32>(event.MouseInput.X, event.MouseInput.Y) ) )
          {
            onHyperlinkClicked();
          }
        }
        break;

      default:
        break;
      }
    }
    break;

  default:
    break;
  }
  return false;
}

///////////////////////////////////////////////////////////////////////////////////////////

void GUIHyperlink::onHyperlinkClicked()
{
  if ( Parent )
  {
    SEvent e;

    e.EventType = EET_GUI_EVENT;
    e.GUIEvent.Caller = this;
    e.GUIEvent.Element = this;
    e.GUIEvent.EventType = static_cast<EGUI_EVENT_TYPE>(EGET_HYPERLINK_CLICKED);
    // send an event
    Parent->OnEvent(e);
  }
}