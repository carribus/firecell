#include "StdAfx.h"
#include "ElementContainer.h"

ElementContainer::ElementContainer(IGUIEnvironment* pEnv, IGUIElement* pParent, s32 id, rect<s32> r)
: IGUIElement(EGUIET_ELEMENT, pEnv, pParent, id, r)
, m_state(ECS_None)
{
#ifdef _DEBUG
  setDebugName("ElementContainer");
#endif//_DEBUG
}

ElementContainer::~ElementContainer(void)
{
}

void ElementContainer::draw()
{
  IrrlichtDevice* pDev = irrSingleton::instance().getDevice();
  IVideoDriver* pDriver = pDev->getVideoDriver();

  if ( pDriver )
  {
    switch ( m_state )
    {
    case  ECS_None:
      pDriver->draw2DRectangle(SColor(32, 255, 0, 0), AbsoluteRect);
      break;

    case  ECS_Hovered:
      pDriver->draw2DRectangle(SColor(32, 255, 255, 0), AbsoluteRect);
      break;

    case  ECS_Selected:
      pDriver->draw2DRectangle(SColor(32, 0, 0, 255), AbsoluteRect);
      break;
    }
    
  }

//  IGUIElement::draw();
}

bool ElementContainer::OnEvent(const SEvent& event)
{
  switch ( event.EventType )
  {
  case  EET_GUI_EVENT:
    {
      switch ( event.GUIEvent.EventType )
      {
      case  EGET_ELEMENT_HOVERED:
        if ( m_state != ECS_Selected )
          m_state = ECS_Hovered;
        break;

      case  EGET_ELEMENT_LEFT:
        if ( m_state != ECS_Selected )
          m_state = ECS_None;
        break;
      }
    }
    break;

  case  EET_MOUSE_INPUT_EVENT:
    {
      switch ( event.MouseInput.Event )
      {
      case  EMIE_LMOUSE_PRESSED_DOWN:
        m_state = (m_state == ECS_Selected ? ECS_None : ECS_Selected);
        return true;

      default:
        break;
      }
    }
    break;
  }
  return false;
}

void ElementContainer::setObject(IGUIElement* pElement, const wchar_t* elemName)
{
  if ( !pElement )
    return;

  pElement->setRelativePosition(AbsoluteRect);;
  pElement->setText(elemName);
  pElement->addChild(this);
}