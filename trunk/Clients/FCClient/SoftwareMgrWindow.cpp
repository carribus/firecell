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

SoftwareMgrWindow::SoftwareMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 530, 340))
, m_model(model)
, m_pDesktop(pDesktop)
{
  NetworkPorts& ports = model.GetPlayer()->GetComputer().GetNetworkPorts();
  FCSHORT portNum = 0, portCount = ports.getPortCount();

  createFormElements();
  setLabels();

  FCDialog::setSuccessCallback(OnOK, this);

  UpdateUIFromModel();

  for ( u32 i = 51; i <= 72; portNum++, i+=3 )
  {
    ((GUIVUMeter*)getElementFromId(i))->setBarColor(SColor(255, 128, 255, 128));
    ((GUIVUMeter*)getElementFromId(i))->setRange(100);
//    ((GUIVUMeter*)getElementFromId(i))->setValue( ports.getPortHealth(portNum) );

//    wstringstream ss;

//    ss << 75 << L" / " << 100;
//    ((IGUIStaticText*)getElementFromId(i+1))->setText(ss.str().c_str());
    ((IGUIStaticText*)getElementFromId(i+1))->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
  }

  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::~SoftwareMgrWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

bool SoftwareMgrWindow::OnButtonClicked(s32 id, IGUIButton* pBtn)
{
  NetworkPorts& ports = m_model.GetPlayer()->GetComputer().GetNetworkPorts();
  FCSHORT portNum = (FCSHORT)(id-50)/3;
  FCSHORT res;
  bool bEnabled = ports.isPortEnabled(portNum);

  bEnabled ^= true;
  res = ports.enablePort(bEnabled);
  switch ( res )
  {
  case  NPE_NO_SERVICE_ASSIGNED:
  case  NPE_NO_SOFTWARETYPE:
/*
    m_pEnv->addMessageBox( L"No service assigned!", 
                           L"You must assign a service to run on this port before you can enable it",
                           true,
                           EMBF_OK);
*/
    break;


  case  NPE_OK:
    ((IGUIButton*)getElementFromId(id))->setPressed( bEnabled );
    break;
  }



  return true;
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

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::UpdateUIFromModel()
{
  NetworkPorts& ports = m_model.GetPlayer()->GetComputer().GetNetworkPorts();
  FCSHORT portCount = ports.getPortCount();
  FCULONG itemID = 0, softwareType = 0;

  for ( FCSHORT i = 0; i < 8; i++ )
  {
    // check if we need to disable any controls
    if ( i+1 > portCount )
    {
      enablePort(i, false);
    }
    else
    {
      // if its not disabled, lets update the port information
      ports.getSoftwareInfo(i, itemID, softwareType);
      setPortInfo( i, itemID, softwareType, ports.isPortEnabled(i), ports.getPortMaxHealth(i), ports.getPortHealth(i) );
    }
  }
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::enablePort(FCSHORT port, bool bEnable)
{
  // Label
  getElementFromId(port*2+1)->setVisible(bEnable);
  // Combo Box
  getElementFromId(port*2+2)->setVisible(bEnable);
  // Enabler Button
  getElementFromId(port*3+50)->setVisible(bEnable);
  // VU Meter
  getElementFromId(port*3+51)->setVisible(bEnable);
  // VU Meter Text
  getElementFromId(port*3+52)->setVisible(bEnable);
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::setPortInfo(FCSHORT port, FCULONG itemID, FCULONG softwareType, bool bEnabled, u32 maxHealth, u32 health)
{
  wstringstream ss;

  // enabled button
  ((IGUIButton*)getElementFromId(port*3+50))->setPressed(bEnabled);
  // VU Meter: Port Health 
  ((GUIVUMeter*)getElementFromId(port*3+51))->setRange( maxHealth );
  ((GUIVUMeter*)getElementFromId(port*3+51))->setValue( health );
  ss << health << L" / " << maxHealth;
  ((IGUIStaticText*)getElementFromId(port*3+52))->setText(ss.str().c_str());
}