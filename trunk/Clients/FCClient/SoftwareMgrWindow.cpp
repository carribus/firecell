#include <sstream>
#include "../common/ResourceManager.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "FCController.h"
#include "FCViewEvent.h"
#include "clientstrings.h"
#include "GUIHyperlink.h"
#include "GUIVUMeter.h"
#include "ItemMgr.h"
#include "SoftwareMgrWindow.h"

#define CTRLID_PORT_1   2
#define CTRLID_PORT_2   4
#define CTRLID_PORT_3   6
#define CTRLID_PORT_4   8
#define CTRLID_PORT_5   10
#define CTRLID_PORT_6   12
#define CTRLID_PORT_7   14
#define CTRLID_PORT_8   16

// menu item definitions
#define MENUITEM_INSTALL_SOFTWARE         1000
#define MENUITEM_UNINSTALL_SOFTWARE       1001

DECLARE_FORM_ELEMENTS(SoftwareMgrWindow)
  FORM_ELEMENT("staticText"			, 1							, 10			, 40			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"				, 2							, 85			, 40			, 250			, 25			, NULL)
  FORM_ELEMENT("hyperlink"			, CTRLID_PORT_1 , 85			, 44			, 250			, 20			, L"")
  FORM_ELEMENT("checkBox"         , 50            , 340     , 40      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 51            , 380     , 40      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 52            , 380     , 53      , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 3							, 10			, 70			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 4							, 85			, 70			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"			, CTRLID_PORT_2 , 85			, 74			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 53            , 340     , 70      , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 54            , 380     , 70      , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 55            , 380     , 83      , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 5							, 10			, 100			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 6							, 85			, 100			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"  	  , CTRLID_PORT_3 , 85			, 104			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 56            , 340     , 100     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 57            , 380     , 100     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 58            , 380     , 113     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 7							, 10			, 130			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 8							, 85			, 130			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"		  , CTRLID_PORT_4 , 85			, 134			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 59            , 340     , 130     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 60            , 380     , 130     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 61            , 380     , 143     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 9							, 10			, 160			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 10						, 85			, 160			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"		  , CTRLID_PORT_5 , 85			, 164			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 62            , 340     , 160     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 63            , 380     , 160     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 64            , 380     , 173     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 11						, 10			, 190			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 12						, 85			, 190			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"		  , CTRLID_PORT_6 , 85			, 194			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 65            , 340     , 190     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 66            , 380     , 190     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 67            , 380     , 203     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 13						, 10			, 220			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 14						, 85			, 220			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"		  , CTRLID_PORT_7 , 85			, 224			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 68            , 340     , 220     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 69            , 380     , 220     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 70            , 380     , 233     , 125     , 12      , L"")

  FORM_ELEMENT("staticText"		  , 15						, 10			, 250			, 70			, 25			, L"")
//  FORM_ELEMENT("comboBox"			  , 16						, 85			, 250			, 250     , 25			, NULL)
  FORM_ELEMENT("hyperlink"		  , CTRLID_PORT_8 , 85			, 254			, 250     , 20			, (wchar_t*)ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str())
  FORM_ELEMENT("checkBox"         , 71            , 340     , 250     , 25      , 25      , L"")
  FORM_ELEMENT("vumeter"        , 72            , 380     , 250     , 125     , 10      , L"")
  FORM_ELEMENT("staticText"     , 73            , 380     , 263     , 125     , 12      , L"")

//  FORM_ELEMENT("hyperlink"      , 200           , 10      , 290     , 175     , 25      , L"This is a hyperlink")
  FORM_ELEMENT("button"					, FCBTN_OK			, 380			, 290			, 125     , 25			, L"")
END_FORM_ELEMENTS();

IMPLEMENT_IRRLICHT_FORM(SoftwareMgrWindow);

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::SoftwareMgrWindow(IDesktop* pDesktop, FCModel& model, IGUIEnvironment* pEnv, wchar_t* caption, IGUIElement* pParent, s32 id)
: FCDialog(pEnv, pParent, caption, true, id, rect<s32>(0, 0, 530, 340))
, m_model(model)
, m_pDesktop(pDesktop)
, m_pController(NULL)
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
    ((IGUIStaticText*)getElementFromId(i+1))->setTextAlignment( EGUIA_CENTER, EGUIA_CENTER );
  }

  CenterWindow();
}

///////////////////////////////////////////////////////////////////////

SoftwareMgrWindow::~SoftwareMgrWindow(void)
{
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::Update()
{
  UpdateUIFromModel();
}

///////////////////////////////////////////////////////////////////////

bool SoftwareMgrWindow::OnCheckboxChanged(s32 id, IGUICheckBox* pBtn)
{
  NetworkPorts& ports = m_model.GetPlayer()->GetComputer().GetNetworkPorts();
  FCSHORT portNum = (FCSHORT)(id-50)/3;
  bool bEnabled = ports.isPortEnabled(portNum);

  // first reset the checkbox to its previous checked state
  // because we need to wait for the server to let us know if its ok to switch states
  ((IGUICheckBox*)getElementFromId(id))->setChecked( bEnabled );
  ((IGUICheckBox*)getElementFromId(id))->setEnabled( false );

  // now flip the state
  bEnabled ^= true;

  // and send it off to the server
  if ( m_pController )
  {
    // notify the server that the player has just enabled/disabled a port
    FCViewEventEnableNetworkPort e(portNum, bEnabled);
    m_pController->OnViewEvent(e);
  }
/*
  FCSHORT res;
  bool bEnabled = ports.isPortEnabled(portNum);

  bEnabled ^= true;
  res = ports.enablePort(portNum, bEnabled);
  switch ( res )
  {
  case  NPE_NO_SERVICE_ASSIGNED:
  case  NPE_NO_SOFTWARETYPE:
    bEnabled = false;
    ports.enablePort(portNum, bEnabled);
    ((IGUICheckBox*)getElementFromId(id))->setChecked( bEnabled );
    break;


  case  NPE_OK:
    {
      ((IGUICheckBox*)getElementFromId(id))->setChecked( bEnabled );
      if ( m_pController )
      {
        // notify the server that the player has just enabled/disabled a port
        FCViewEventEnableNetworkPort e(portNum, bEnabled);
        m_pController->OnViewEvent(e);
      }
    }
    break;
  }
*/
  return true;
}

///////////////////////////////////////////////////////////////////////

bool SoftwareMgrWindow::OnMenuItemSelected(s32 selectedItem, IGUIContextMenu* pMenu)
{
  if ( !pMenu )
    return false;

  if ( m_pController )
  {
    s32 cmdID = pMenu->getItemCommandId(selectedItem);

    switch ( cmdID )
    {
    case  MENUITEM_INSTALL_SOFTWARE:
      {
        stMenuItem menuItem = m_menuItems[selectedItem];
        ItemMgr::GameItem item;
        FCViewEventInstallSoftware e(menuItem.itemID, menuItem.targetPort);
        m_pController->OnViewEvent(e);
      }
      break;

    case  MENUITEM_UNINSTALL_SOFTWARE:
      {
        // uninstall a port here
        stMenuItem menuItem = m_menuItems[m_menuItems.size()-1];
        FCViewEvent e(VE_UninstallSoftware, menuItem.targetPort);
        m_pController->OnViewEvent(e);
      }
      break;

    default:
      break;
    }
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool SoftwareMgrWindow::OnUnknownGUIEvent(u32 eventType, s32 id, IGUIElement* pElem)
{
  switch ( eventType )
  {
  case  EGET_HYPERLINK_CLICKED:
    {
      s32 id = pElem->getID();
      FCSHORT portNum = (FCSHORT)id/2 - 1;

      switch ( id )
      {
      case  CTRLID_PORT_1:
      case  CTRLID_PORT_2:
      case  CTRLID_PORT_3:
      case  CTRLID_PORT_4:
      case  CTRLID_PORT_5:
      case  CTRLID_PORT_6:
      case  CTRLID_PORT_7:
      case  CTRLID_PORT_8:
        {
          popupPortMenu(portNum);
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

  pElem = getElementFromId(FCBTN_OK);
  pElem->setText(ResourceManager::instance().GetClientString(STR_DLG_CLOSE).c_str());
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
  // hyperlink
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
  ItemMgr& itemMgr = FCModel::instance().GetItemMgr();
  ItemMgr::GameItem item; 
  ItemSoftware* pItem = NULL;

  // software details
  itemMgr.getItem(itemID, item);
  pItem = (ItemSoftware*)item.getItem();
  if ( pItem )
  {
    ss << pItem->GetName().c_str();
  }
  else
  {
    ss << ResourceManager::instance().GetClientString(STR_SWM_CONFIGURE_PORT).c_str();
  }
  ((GUIHyperlink*)getElementFromId((port+1)*2))->setText( ss.str().c_str() );
  ss.str(L"");
  // enabled button
  ((IGUICheckBox*)getElementFromId(port*3+50))->setEnabled(true);
  ((IGUICheckBox*)getElementFromId(port*3+50))->setChecked(bEnabled);
  // VU Meter: Port Health 
  ((GUIVUMeter*)getElementFromId(port*3+51))->setRange( maxHealth );
  ((GUIVUMeter*)getElementFromId(port*3+51))->setValue( health );
  ss << health << L" / " << maxHealth;
  ((IGUIStaticText*)getElementFromId(port*3+52))->setText(ss.str().c_str());
}

///////////////////////////////////////////////////////////////////////

void SoftwareMgrWindow::popupPortMenu(FCSHORT portNum)
{
  ItemMgr& itemMgr = FCModel::instance().GetItemMgr();
  NetworkPorts& ports = m_model.GetPlayer()->GetComputer().GetNetworkPorts();
  std::vector<ItemMgr::GameItem> services;
  std::wstringstream ss;
  FCULONG itemID, softwareType;
  IGUIContextMenu* pMenu = NULL, *pSubMenu = NULL;
  u32 menuIdx = 0;
  stMenuItem menuItem;
  core::rect<s32> r = getElementFromId(portNum*2+2)->getRelativePosition();

  // clear the menu item vector
  m_menuItems.clear();

  // get all service items available to the player
  itemMgr.getServices(services);

  // get information on the port
  ports.getSoftwareInfo(portNum, itemID, softwareType);

	r.UpperLeftCorner.Y = r.LowerRightCorner.Y;
	r.LowerRightCorner.X = r.UpperLeftCorner.X + 200;
	r.LowerRightCorner.Y = r.UpperLeftCorner.Y + 300;
 
  pMenu = m_pEnv->addContextMenu(r, this);
  
  menuIdx = pMenu->addItem(ResourceManager::instance().GetClientString(STR_SWM_INSTALL_SOFTWARE).c_str(), -1, true, true);
  if ( (pSubMenu = pMenu->getSubMenu(menuIdx)) )
  {
    std::vector<ItemMgr::GameItem>::iterator it = services.begin();
    std::vector<ItemMgr::GameItem>::iterator limit = services.end();

    for ( ; it != limit; ++it )
    {
      if ( it->getCount() > 0 )
      {
        Item* pItem = it->getItem();
        // create the menu item's text
        ss << pItem->GetName().c_str();
        // create the menuitem data object
        menuItem.targetPort = portNum;
        menuItem.itemID = pItem->GetID();
        menuItem.menuIndex = pSubMenu->addItem( ss.str().c_str(), 
                                                MENUITEM_INSTALL_SOFTWARE );
        // add the menuitem data object to our vector
        m_menuItems.push_back(menuItem);

        ss.str(L"");
      }
    }
  }
  pMenu->addItem(ResourceManager::instance().GetClientString(STR_SWM_UNINSTALL_SOFTWARE).c_str(), MENUITEM_UNINSTALL_SOFTWARE, itemID > 0);
  memset(&menuItem, 0, sizeof(menuItem));
  menuItem.targetPort = portNum;
  m_menuItems.push_back(menuItem);

	r.LowerRightCorner.X += 10;
  r.UpperLeftCorner.X += 10;
  pMenu->setRelativePosition(r);

  m_pEnv->setFocus(pMenu);
}