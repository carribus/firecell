#ifndef _FCDIALOG_H_
#define _FCDIALOG_H_

#include <irrlicht.h>

using namespace irr;
using namespace core;
using namespace scene;
using namespace gui;
using namespace video;

class FCDialog : public IGUIElement
{
	typedef void(*DLG_CALLBACK)(void);

public:
  FCDialog(IGUIEnvironment* env, IGUIElement* pParent = NULL, wchar_t* caption = NULL, bool bModal = false, s32 id = -1);
  ~FCDialog(void);

	void setSuccessCallback(DLG_CALLBACK fpCallback)								{ m_fpSuccessCallback = fpCallback; }
	void setCancelCallback(DLG_CALLBACK fpCallback)									{ m_fpCancelCallback = fpCallback; }
	bool loadGUI(const char* filename);
	void draw();
  bool OnEvent(const SEvent& event);

protected:

	virtual bool OnButtonClicked(s32 id, IGUIButton* pBtn) { return false; }
	virtual bool OnCheckboxChanged(s32 id, IGUICheckBox* pCB) { return false; }
	virtual bool OnComboBoxChanged(s32 id, IGUIComboBox* pCB) { return false; }
	virtual bool OnEditBoxEnter(s32 id, IGUIEditBox* pEdit) { return false; }
	virtual bool OnListBoxChanged(s32 id, IGUIListBox* pLB) { return false; }
	virtual bool OnListBoxSelectedAgain(s32 id, IGUIListBox* pLB) { return false; }
	virtual bool OnScrollBarChanged(s32 id, IGUIScrollBar* pSB) { return false; }
	virtual bool OnSpinBoxChanged(s32 id, IGUISpinBox* pSB) { return false; }

private:

	//! sends the event that the file choose process has been canceld
	void closeDialog();

  bool									m_bModal;
	core::position2d<s32> DragStart;
	bool									Dragging;
	IGUIButton*						m_CloseButton;
	DLG_CALLBACK					m_fpSuccessCallback,
												m_fpCancelCallback;
};

#endif//_FCDIALOG_H_