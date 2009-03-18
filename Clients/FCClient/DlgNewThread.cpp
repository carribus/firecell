#include "DlgNewThread.h"

DECLARE_FORM_ELEMENTS(DlgNewThread)
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 90			, 25			, L"Subject : ")
  FORM_ELEMENT("editBox"				, 2							, 105			, 40			, 250			, 25			, NULL)
  FORM_ELEMENT("staticText"		  , 3							, 10			, 80			, 90			, 25			, L"Message : ")
  FORM_ELEMENT("editBox"			  , 4							, 10			, 100			, 400			, 300			, NULL)
  FORM_ELEMENT("button"					, FCBTN_CANCEL	, 10			, 410			, 200			, 25			, L"Cancel")
  FORM_ELEMENT("button"					, FCBTN_OK			, 210			, 410			, 200			, 25			, L"Post Message")
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(DlgNewThread);

DlgNewThread::DlgNewThread(IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 425, 450))
{
	// create the controls of the object
  createFormElements();

	// update the message edit box settings
	IGUIEditBox* pEB = (IGUIEditBox*) getElementFromId(4);
	if ( pEB )
	{
		pEB->setMultiLine(true);
		pEB->setTextAlignment( EGUIA_UPPERLEFT, EGUIA_UPPERLEFT );
		pEB->setWordWrap(true);
	}

  Environment->setFocus( getElementFromId(2) );
  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

DlgNewThread::~DlgNewThread(void)
{
}

///////////////////////////////////////////////////////////////////////

bool DlgNewThread::OnButtonClicked(s32 id, IGUIButton* pBtn)
{
  switch ( id )
  {
  case  FCBTN_OK:
    OnOK();
    break;

  case  FCBTN_CANCEL:
    OnCancel();
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

void DlgNewThread::OnOK()
{
	IGUIEditBox* pEB = (IGUIEditBox*) getElementFromId(2);

	m_strSubject = pEB->getText();
	pEB = (IGUIEditBox*) getElementFromId(4);
	m_strMessage = pEB->getText();

	FCDialog::OnOK();
}

///////////////////////////////////////////////////////////////////////

void DlgNewThread::OnCancel()
{
	FCDialog::OnCancel();
}
