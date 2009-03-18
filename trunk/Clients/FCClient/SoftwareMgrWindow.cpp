#include <sstream>
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "SoftwareMgrWindow.h"

DECLARE_FORM_ELEMENTS(SoftwareMgrWindow)
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"				, 2							, 105			, 40			, 250			, 25			, NULL)
  FORM_ELEMENT("staticText"		  , 3							, 10			, 70			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 4							, 105			, 70			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 5							, 10			, 100			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 6							, 105			, 100			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 7							, 10			, 130			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 8							, 105			, 130			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 9							, 10			, 160			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 10						, 105			, 160			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 11						, 10			, 190			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 12						, 105			, 190			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 13						, 10			, 220			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 14						, 105			, 220			, 250     , 25			, NULL)
  FORM_ELEMENT("staticText"		  , 15						, 10			, 250			, 90			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 16						, 105			, 250			, 250     , 25			, NULL)
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(SoftwareMgrWindow);

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::SoftwareMgrWindow(IDesktop* pDesktop, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 550, 400))
, m_pDesktop(pDesktop)
{
  createFormElements();
  setLabels();

  FCDialog::setSuccessCallback(OnOK, this);

  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::~SoftwareMgrWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::OnOK()
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::OnCancel()
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::setLabels()
{
  wstringstream ss;
  wstring label = ResourceManager::instance().GetClientString( STR_SWM_LABEL_PORT );
  IGUIElement* pElem = NULL;

  for ( int i = 0; i < 8; i++ )
  {
    if ( (pElem = getElementFromId(i*2+1)) )
    {
      ss << label.c_str() << i;
      pElem->setText( ss.str().c_str() );
      ss.str(L"");
    }
  }
}