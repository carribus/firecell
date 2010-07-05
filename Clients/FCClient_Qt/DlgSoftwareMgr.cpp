#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include <QMenu>
#include "FCApp.h"
#include "clientstrings.h"
#include "ResourceManager.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "FCPlayerModel.h"
#include "DlgSoftwareMgr.h"

DlgSoftwareMgr::DlgSoftwareMgr(QWidget* parent)
: QDialog(parent)
{
  ui.setupUi(this);
  this->setWindowTitle( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_SOFTWAREMGR ) );

  // connect the signals
  for ( int i = 0; i < 8; i++ )
  {
    QLabel* pLabel = findChild<QLabel*>( QString("lnkSoftware%1").arg(i) );
    connect( pLabel, SIGNAL(linkActivated(const QString&)), SLOT(onLinkActivated(const QString&)) );
  }

  updateUIFromModel();
}

///////////////////////////////////////////////////////////////////////

DlgSoftwareMgr::~DlgSoftwareMgr(void)
{
}

///////////////////////////////////////////////////////////////////////

void DlgSoftwareMgr::onLinkActivated(const QString& link)
{
  qDebug() << link;

  FCPlayerModel* player = FCAPP->playerModel();
  ItemMgr& itemMgr = player->itemMgr();
  NetworkPorts& ports = player->getCurrentCharacter()->GetComputer().GetNetworkPorts();
  std::vector<ItemMgr::GameItem> services;
  FCULONG itemID, softwareType;
  FCSHORT port = 0;
  QString theLink = link;
  
  // figure out which port this is
  if ( theLink.indexOf("showSoftware") == 0 )
  {
    theLink.remove("showSoftware");
    port = theLink.toUShort();
    // get all the services available to the player
    itemMgr.getServices(services);

    // get information on the port
    ports.getSoftwareInfo(port, itemID, softwareType);

    QMenu menu(this);
    QMenu* pSubMenu = menu.addMenu( ResourceManager::instance().getClientString(STR_SWM_INSTALL_SOFTWARE) );

    menu.addAction( ResourceManager::instance().getClientString( STR_SWM_UNINSTALL_SOFTWARE ) );
    
    // create the sub menu for software that can be installed
    if ( pSubMenu )
    {
      std::vector<ItemMgr::GameItem>::iterator it = services.begin();
      std::vector<ItemMgr::GameItem>::iterator limit = services.end();

      for ( ; it != limit; ++it )
      {
        if ( it->getCount() > 0 )
        {
          Item* pItem = it->getItem();
          QAction* pAction = pSubMenu->addAction( QString(pItem->GetName().c_str()) /*, this, SLOT(onInstallSoftware */ );

          if ( pAction )
          {
            QVariant v( (qulonglong) pItem->GetID() );
            pAction->setData( v );
          }
        }
      }
    }

    QAction* pSelAction = menu.exec(QCursor::pos());

    // if the user chose to install software...
    if ( pSelAction )
    {
      QVariant v = pSelAction->data();
      FCULONG itemID = v.value<FCULONG>();
      Item* pItem = NULL;
      ItemMgr::GameItem item;

      if ( v.type() != QVariant::Invalid )
      {
        itemMgr.getItem( itemID, item );
        qDebug() << item.getItem()->GetName().c_str();
      }

    }

  }
}

///////////////////////////////////////////////////////////////////////

void DlgSoftwareMgr::updateUIFromModel()
{
  FCPlayerModel* player = FCAPP->playerModel();
  NetworkPorts& ports = player->getCurrentCharacter()->GetComputer().GetNetworkPorts();
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

void DlgSoftwareMgr::enablePort(FCSHORT port, bool bEnable)
{
  // hide/show all the controls for the port
}

///////////////////////////////////////////////////////////////////////

void DlgSoftwareMgr::setPortInfo(FCSHORT port, FCULONG itemID, FCULONG softwareType, bool bEnabled, quint32 maxHealth, quint32 health)
{
  QString str;
  ItemMgr& itemMgr = FCAPP->playerModel()->itemMgr();
  ItemMgr::GameItem item; 
  ItemSoftware* pItem = NULL;

  // software details
  itemMgr.getItem(itemID, item);
  pItem = (ItemSoftware*)item.getItem();
  if ( pItem )
  {
    str = pItem->GetName().c_str();
  }
  else
  {
    str = ResourceManager::instance().getClientString(STR_SWM_CONFIGURE_PORT);
  }

  // update the checkbox
  QCheckBox* pCB = findChild<QCheckBox*>( QString("cbPort%1").arg(port) );
  if ( pCB )
  {
    pCB->setEnabled( true );
    pCB->setChecked( bEnabled );
  }
  // update the software link
  QLabel* pLabel = findChild<QLabel*>( QString("lnkSoftware%1").arg(port) );
  if ( pLabel )
  {
    pLabel->setText( QString("<a href=\"showSoftware:%1\">%2</a>").arg(port).arg(str) );
  }
  // update the health bar
  HealthMeterWidget* pHealth = findChild<HealthMeterWidget*>( QString("healthBar%1").arg(port) );
  if ( pHealth )
  {
    pHealth->setMaxValue( maxHealth );
    pHealth->setCurrentValue( health );
  }

  
/*
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
*/
}