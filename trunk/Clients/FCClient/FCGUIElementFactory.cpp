#include "GUIConsoleCtrl.h"
#include "FCGUIElementFactory.h"

#ifdef _IRR_COMPILE_WITH_GUI_

#include "IGUIEnvironment.h"

FCGUIElementFactory::TypeToName FCGUIElementFactory::m_arrTypeToName[] = 
{
  { FCGUIElementFactory::EGUIET_CONSOLE, "console" },
  { (FCGUIElementFactory::e_FCGUI_ElementType)0, NULL }
};

FCGUIElementFactory::FCGUIElementFactory(IGUIEnvironment* env)
: m_pEnv(env)
, m_creatableTypeCount(0)
{
	#ifdef _DEBUG
	setDebugName("FCGUIElementFactory");
	#endif

  // count the number of creatable objects in this factory
  TypeToName* p = m_arrTypeToName;
  while ( p->type )
  {
    m_creatableTypeCount++;
    p++;
  }
}

FCGUIElementFactory::~FCGUIElementFactory(void)
{
}

//! adds an element to the gui environment based on its type id
/** \param type: Type of the element to add.
\param parent: Parent scene node of the new element, can be null to add to the root.
\return Pointer to the new element or null if not successful. */
IGUIElement* FCGUIElementFactory::addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent)
{
  IGUIElement* pRet = NULL;

  switch ( type )
  {
  case  EGUIET_CONSOLE:
    pRet = new GUIConsoleCtrl(m_pEnv, core::rect<s32>(0, 0, 100, 100),  parent);
    break;

  default:
    break;
  }

  return pRet;
}

//! adds a GUI element to the GUI Environment based on its type name
/** \param typeName: Type name of the element to add.
\param parent: Parent scene node of the new element, can be null to add it to the root.
\return Pointer to the new element or null if not successful. */
IGUIElement* FCGUIElementFactory::addGUIElement(const c8* typeName, IGUIElement* parent)
{
	return addGUIElement( (EGUI_ELEMENT_TYPE)getTypeFromName(typeName), parent );
}

//! Get amount of GUI element types this factory is able to create
s32 FCGUIElementFactory::getCreatableGUIElementTypeCount() const
{
  return m_creatableTypeCount;
}

//! Get type of a createable element type
/** \param idx: Index of the element type in this factory. Must be a value between 0 and
getCreatableGUIElementTypeCount() */
EGUI_ELEMENT_TYPE FCGUIElementFactory::getCreateableGUIElementType(s32 idx) const
{
  s32 c = 0;
  TypeToName* p = m_arrTypeToName;
  for ( ; p->type; p++, c++ )
  {
    if ( c == idx )
      return (EGUI_ELEMENT_TYPE)p->type;
  }

  // default response
  return EGUIET_ELEMENT;
}

//! Get type name of a createable GUI element type by index
/** \param idx: Index of the type in this factory. Must be a value between 0 and
getCreatableGUIElementTypeCount() */
const c8* FCGUIElementFactory::getCreateableGUIElementTypeName(s32 idx) const
{
  s32 c = 0;
  TypeToName* p = m_arrTypeToName;
  for ( ; p->type; p++, c++ )
  {
    if ( c == idx )
      return p->name;
  }

  return NULL;
}

//! returns type name of a createable GUI element
/** \param type: Type of GUI element.
\return Name of the type if this factory can create the type, otherwise 0. */
const c8* FCGUIElementFactory::getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const
{
  TypeToName* p = m_arrTypeToName;
  for ( ; p->type; p++ )
  {
    if ( p->type == type )
      return p->name;
  }

  return NULL;
}

FCGUIElementFactory::e_FCGUI_ElementType FCGUIElementFactory::getTypeFromName(const c8* name) const
{
  TypeToName* p = m_arrTypeToName;
  for ( ; p->type; p++ )
  {
    if ( !strcmp(name, p->name) )
      return p->type;
  }

  return (e_FCGUI_ElementType)EGUIET_ELEMENT;
}
#endif// #ifdef _IRR_COMPILE_WITH_GUI_

