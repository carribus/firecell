#ifndef _FCDIALOG_H_
#define _FCDIALOG_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;
using namespace video;

/**
 *  \brief  This macro must be included inside the definition of a FCDialog derived class. It defines
 *          the structure required to add elements to the dialog, as well as the functions that are 
 *          implemented by the IMPLEMENT_IRRLICH_FORM() macro
 */
#define DEFINE_IRRLICHT_FORM()          \
  private:                              \
	struct FormElement                    \
	{                                     \
		char*								elemType;       \
		s32									id;             \
		s32									xPos,           \
												yPos,           \
												width,          \
												height;         \
		wchar_t*						text;           \
	};                                    \
	static FormElement		m_arrFormElements[];      \
  void createFormElements(); \
  IGUIElement* createFormElement(FormElement& fe);

/**
 *  \brief  This macro must be used in the implementation file of the FCDialog derived class. It provides
 *          the implementation of the functions declared by DEFINE_IRRLICHT_FORM()
 */
#define IMPLEMENT_IRRLICHT_FORM(className)            \
  void className::createFormElements() \
  { \
	  FormElement* pFE = m_arrFormElements; \
	  while ( pFE->elemType ) \
	  { \
		  createFormElement(*pFE); \
		  pFE++; \
	  } \
  } \
  IGUIElement* className::createFormElement(FormElement& fe)  \
  { \
    IGUIElement* pElem = Environment->addGUIElement( fe.elemType, this ); if ( pElem ) \
    { \
      pElem->setID( fe.id ); \
      pElem->setRelativePosition( core::rect<s32>( fe.xPos, fe.yPos, fe.xPos + fe.width, fe.yPos + fe.height ) ); \
      pElem->setText( fe.text ); \
      if ( pElem->getType() == EGUIET_STATIC_TEXT ) \
      { \
        ((IGUIStaticText*)pElem)->setTextAlignment( EGUIA_LOWERRIGHT, EGUIA_CENTER ); \
      } \
    } \
  return pElem; \
  }

/**
 *  \brief  This macro should be used at the top of the implementation file of the FCDialog derived class. 
 *          Its purpose is to populate the m_arrFormElements data member with the element definitions of the 
 *          objects that are visible on the dialog.
 */
#define DECLARE_FORM_ELEMENTS(className)  \
className::FormElement className::m_arrFormElements[] = \
{

/**
 *  \brief  This macro is to be used within the DECLARE_FORM_ELEMENTS() macro, and it will define a single
 *          form element.
 */
#define FORM_ELEMENT(objTypeName, id, xpos, ypos, width, height, text)  \
  { (objTypeName), (id), (xpos), (ypos), (width), (height), (text) },

/**
 *  \brief  This macro indicates the end of the form elements definition block. This must be used after all
 *          FORM_ELEMENT() macros have been defined, to end off the block started with DECLARE_FORM_ELEMENTS()
 */
#define END_FORM_ELEMENTS() \
  { NULL						, -1	, 0			, 0			, 0				, 0				, NULL }  \
};

/**
 *  Standard button ID definitions
 */
#define FCBTN_OK            0x0000FFFE
#define FCBTN_CANCEL        0x0000FFFF

/**
 *  \class  FCDialog
 *  \brief  This object was designed to serve as a 'real' dialog class for irrLicht GUI development. 
 *          It supports modal operation, as well as cancel/complete callbacks
 */
class FCDialog : public IGUIElement
{
	typedef void(*DLG_CALLBACK)(void*);

public:
  FCDialog(IGUIEnvironment* env, IGUIElement* pParent = NULL, wchar_t* caption = NULL, bool bModal = false, s32 id = -1, core::rect<s32> rect = core::rect<s32>(0, 0, 320, 240));
  ~FCDialog(void);

  void CenterWindow();

	virtual void setSuccessCallback(DLG_CALLBACK fpCallback, void* pCtx)								{ m_fpSuccessCallback = fpCallback; m_pSuccessCtx = pCtx; }
	virtual void setCancelCallback(DLG_CALLBACK fpCallback, void* pCtx)									{ m_fpCancelCallback = fpCallback; m_pCancelCtx = pCtx; }
//	bool loadGUI(const char* filename);
	virtual void draw();
  virtual bool OnEvent(const SEvent& event);

protected:

  virtual void createFormElements() {}
	virtual bool OnButtonClicked(s32 id, IGUIButton* pBtn) { return false; }
	virtual bool OnCheckboxChanged(s32 id, IGUICheckBox* pCB) { return false; }
	virtual bool OnComboBoxChanged(s32 id, IGUIComboBox* pCB) { return false; }
	virtual bool OnEditBoxEnter(s32 id, IGUIEditBox* pEdit) { return false; }
	virtual bool OnListBoxChanged(s32 id, IGUIListBox* pLB) { return false; }
	virtual bool OnListBoxSelectedAgain(s32 id, IGUIListBox* pLB) { return false; }
	virtual bool OnScrollBarChanged(s32 id, IGUIScrollBar* pSB) { return false; }
	virtual bool OnSpinBoxChanged(s32 id, IGUISpinBox* pSB) { return false; }

  void OnOK();
  void OnCancel();

private:

	//! sends the event that the file choose process has been canceld
	void closeDialog();

  bool									m_bModal;
	core::position2d<s32> DragStart;
	bool									Dragging;
	IGUIButton*						m_CloseButton;
	DLG_CALLBACK					m_fpSuccessCallback,
												m_fpCancelCallback;
  void*                 m_pSuccessCtx;
  void*                 m_pCancelCtx;
};

#endif//_FCDIALOG_H_