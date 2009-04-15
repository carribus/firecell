#ifndef _FCGUIELEMENTFACTORY_H_
#define _FCGUIELEMENTFACTORY_H_

#include "IrrCompileConfig.h"
#ifdef _IRR_COMPILE_WITH_GUI_

#include <IGUIElementFactory.h>

using namespace irr;
using namespace gui;

namespace irr
{
namespace gui
{
  class IGUIElement;
	class IGUIEnvironment;

  class FCGUIElementFactory : public IGUIElementFactory
  {
  public:

    /**
     *  \brief Enumeration of GUI elements that this factory can create
     */
    enum e_FCGUI_ElementType
    {
      EGUIET_CONSOLE = (EGUIET_ELEMENT+1),        /**<A terminal-like console control */
      EGUIET_VUMETER,                             /**<A VU Meter control */
      EGUIET_HYPERLINK                            /**<A Hyperlink control */
    };

    FCGUIElementFactory(IGUIEnvironment* env);
    ~FCGUIElementFactory(void);

		//! adds an element to the gui environment based on its type id
		/** \param type: Type of the element to add.
		\param parent: Parent scene node of the new element, can be null to add to the root.
		\return Pointer to the new element or null if not successful. */
		IGUIElement* addGUIElement(EGUI_ELEMENT_TYPE type, IGUIElement* parent=0);

		//! adds a GUI element to the GUI Environment based on its type name
		/** \param typeName: Type name of the element to add.
		\param parent: Parent scene node of the new element, can be null to add it to the root.
		\return Pointer to the new element or null if not successful. */
		IGUIElement* addGUIElement(const c8* typeName, IGUIElement* parent=0);

		//! Get amount of GUI element types this factory is able to create
		s32 getCreatableGUIElementTypeCount() const;

		//! Get type of a createable element type
		/** \param idx: Index of the element type in this factory. Must be a value between 0 and
		getCreatableGUIElementTypeCount() */
		EGUI_ELEMENT_TYPE getCreateableGUIElementType(s32 idx) const;

		//! Get type name of a createable GUI element type by index
		/** \param idx: Index of the type in this factory. Must be a value between 0 and
		getCreatableGUIElementTypeCount() */
		const c8* getCreateableGUIElementTypeName(s32 idx) const;

		//! returns type name of a createable GUI element
		/** \param type: Type of GUI element.
		\return Name of the type if this factory can create the type, otherwise 0. */
		const c8* getCreateableGUIElementTypeName(EGUI_ELEMENT_TYPE type) const;

  private:

 		e_FCGUI_ElementType getTypeFromName(const c8* name) const;


    IGUIEnvironment*        m_pEnv;

    /*
     * structure to allow type-from-name semantics to work
     */
    struct TypeToName
    {
      e_FCGUI_ElementType type;
      char* name;
    };
    static TypeToName       m_arrTypeToName[];
    s32                     m_creatableTypeCount;
	};

} // end namespace gui
} // end namespace irr

#endif // _IRR_COMPILE_WITH_GUI_

#endif//_FCGUIELEMENTFACTORY_H_