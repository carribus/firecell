#include <sstream>
#include "../common/ResourceManager.h"
#include "clientstrings.h"
#include "GUIVUMeter.h"
#include "SoftwareMgrWindow.h"

DECLARE_FORM_ELEMENTS(SoftwareMgrWindow)
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"				, 2							, 85			, 40			, 250			, 25			, NULL)
  FORM_ELEMENT("button"         , 50            , 340     , 40      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 51            , 380     , 40      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 52            , 380     , 53      , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 3							, 10			, 70			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 4							, 85			, 70			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 53            , 340     , 70      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 54            , 380     , 70      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 55            , 380     , 83      , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 5							, 10			, 100			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 6							, 85			, 100			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 56            , 340     , 100     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 57            , 380     , 100     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 58            , 380     , 113     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 7							, 10			, 130			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 8							, 85			, 130			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 59            , 340     , 130     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 60            , 380     , 130     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 61            , 380     , 143     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 9							, 10			, 160			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 10						, 85			, 160			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 62            , 340     , 160     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 63            , 380     , 160     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 64            , 380     , 173     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 11						, 10			, 190			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 12						, 85			, 190			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 65            , 340     , 190     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 66            , 380     , 190     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 67            , 380     , 203     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 13						, 10			, 220			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 14						, 85			, 220			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 68            , 340     , 220     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 69            , 380     , 220     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 70            , 380     , 233     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 15						, 10			, 250			, 70			, 25			, L"")
  FORM_ELEMENT("comboBox"			  , 16						, 85			, 250			, 250     , 25			, NULL)
  FORM_ELEMENT("button"         , 71            , 340     , 250     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 72            , 380     , 250     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 73            , 380     , 263     , 125     , 12      , L"")

  FORM_ELEMENT("button"					, FCBTN_OK			, 380			, 290			, 125     , 25			, L"Close")
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(SoftwareMgrWindow);

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::SoftwareMgrWindow(IDesktop* pDesktop, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 530, 340))
, m_pDesktop(pDesktop)
{
  createFormElements();
  setLabels();

  FCDialog::setSuccessCallback(OnOK, this);

  for ( u32 i = 51; i <= 72; i+=3 )
  {
    ((GUIVUMeter*)getElementFromId(i))->setBarColor(SColor(255, 128, 255, 128));
    ((GUIVUMeter*)getElementFromId(i))->setRange(100);
    ((GUIVUMeter*)getElementFromId(i))->setValue(75);

    wstringstream ss;

    ss << 75 << L" / " << 100;
    ((IGUIStaticText*)getElementFromId(i+1))->setText(ss.str().c_str());
    ((IGUIStaticText*)getElementFromId(i+1))->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
  }

  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::~SoftwareMgrWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::OnOK(void* pCtx)
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::OnCancel(void* pCtx)
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
      ss << label.c_str() << i << L":";
      pElem->setText( ss.str().c_str() );
      ss.str(L"");
    }
  }
}