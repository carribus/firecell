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
    if ( pLabel )
      connect( pLabel, SIGNAL(linkActivated(const QString&)), SLOT(onLinkActivated(const QString&)) );


    QCheckBox* pCB = findChild<QCheckBox*>( QString("cbPort%1").arg(i) );
    if ( pCB )
    {
      m_lstCheckboxes << pCB;
      pCB->installEventFilter(this);
    }
  }

  updateUIFromModel();

  // connect the player to the signals emitted from this dialog
  FCPlayerModel* player = FCAPP->playerModel();
  connect(this, SIGNAL(installSoftware(FCSHORT, FCULONG)), player, SLOT(onInstallSoftware(FCSHORT, FCULONG)));
  connect(this, SIGNAL(uninstallSoftware(FCSHORT)), player, SLOT(onUninstallSoftware(FCSHORT)));
  connect(this, SIGNAL(enableSoftwarePort(FCSHORT, bool)), player, SLOT(onEnableSoftwarePort(FCSHORT, bool)));

  // connect to software signals emited by the player model
  connect(player, SIGNAL(softwareInstallSucceeded(FCSHORT, FCULONG)), this, SLOT(updateUIFromModel()));
  connect(player, SIGNAL(softwareInstallFailed(FCSHORT, FCULONG)), this, SLOT(updateUIFromModel()));
  connect(player, SIGNAL(softwareUninstallSucceeded(FCSHORT)), this, SLOT(updateUIFromModel()));
  connect(player, SIGNAL(softwareUninstallFailed(FCSHORT)), this, SLOT(updateUIFromModel()));
  connect(player, SIGNAL(networkPortStatusChangeSucceeded(bool, FCSHORT)), this, SLOT(updateUIFromModel()));
  connect(player, SIGNAL(networkPortStatusChangeFailed(FCSHORT, bool, FCSHORT)), this, SLOT(updateUIFromModel()));
}

///////////////////////////////////////////////////////////////////////

DlgSoftwareMgr::~DlgSoftwareMgr(void)
{
  // disconnect the FCPlayerModel slots
  FCPlayerModel* player = FCAPP->playerModel();
  disconnect(this, SIGNAL(installSoftware(short, FCULONG)), player, SLOT(onInstallSoftware(short, FCULONG)));
  disconnect(this, SIGNAL(uninstallSoftware(short)), player, SLOT(onUninstallSoftware(short)));
  disconnect(this, SIGNAL(enableSoftwarePort(FCSHORT, bool)), player, SLOT(onEnableSoftwarePort(FCSHORT, bool)));

  disconnect(player, SIGNAL(softwareInstallSucceeded(FCSHORT, FCULONG)), this, SLOT(updateUIFromModel()));
  disconnect(player, SIGNAL(softwareInstallFailed(FCSHORT, FCULONG)), this, SLOT(updateUIFromModel()));
  disconnect(player, SIGNAL(softwareUninstallSucceeded(FCSHORT)), this, SLOT(updateUIFromModel()));
  disconnect(player, SIGNAL(softwareUninstallFailed(FCSHORT)), this, SLOT(updateUIFromModel()));
  disconnect(player, SIGNAL(networkPortStatusChangeSucceeded(bool, FCSHORT)), this, SLOT(updateUIFromModel()));
  disconnect(player, SIGNAL(networkPortStatusChangeFailed(FCSHORT, bool, FCSHORT)), this, SLOT(updateUIFromModel()));
}

///////////////////////////////////////////////////////////////////////

bool DlgSoftwareMgr::eventFilter(QObject* obj, QEvent* event)
{
  // we're monitoring for the user clicking the checkboxes to enable/disable ports...
  // first check the event type -- we're only interested in the QEvent::MouseButtonRelease event
  if ( event->type() == QEvent::MouseButtonRelease )
  {
    // check if this event was from one of our checkboxes
    if ( m_lstCheckboxes.contains( reinterpret_cast<QCheckBox*&>(obj) ) )
    {
      obj->event(event);
      return onPortCheckboxClicked(obj);
    }
  }

  return QObject::eventFilter(obj, event);
}

///////////////////////////////////////////////////////////////////////

bool DlgSoftwareMgr::onPortCheckboxClicked(QObject* obj)
{
  bool bResult = false;

  QCheckBox* pCB = static_cast<QCheckBox*>(obj);
  QString objName;

  if ( pCB )
  {
    objName = pCB->objectName();
    if ( objName.indexOf("cbPort") == 0 )
    {
      objName.remove(0, 6);
      short port = objName.toUShort();
      if ( port >= 0 && port <= 7 )
      {
        switch ( pCB->checkState() )
        {
        case  Qt::Checked:
          emit enableSoftwarePort(port, true);
          break;

        case  Qt::Unchecked:
          emit enableSoftwarePort(port, false);
          break;

        default:
          break;
        }
      }
    }
  }

  return bResult;
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
  if ( theLink.indexOf("showSoftware:") == 0 )
  {
    theLink.remove("showSoftware:");
    port = theLink.toUShort();
    // get all the services available to the player
    itemMgr.getServices(services);

    // get information on the port
    ports.getSoftwareInfo(port, itemID, softwareType);

    QMenu menu(this);
    QMenu* pSubMenu = menu.addMenu( ResourceManager::instance().getClientString(STR_SWM_INSTALL_SOFTWARE) );

    QAction* pUninstAction = menu.addAction( ResourceManager::instance().getClientString( STR_SWM_UNINSTALL_SOFTWARE ) );
    if ( pUninstAction )
    {
      pUninstAction->setEnabled( itemID != 0 );
    }
    
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
          QAction* pAction = pSubMenu->addAction( QString(pItem->GetName().c_str()) );

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

      if ( v.type() != QVariant::Invalid )
      {
        FCULONG itemID = v.value<FCULONG>();

        emit installSoftware(port, itemID);
      }
      else
      {
        // this is the uninstall handler (since the uninstall option is the only option that has a invalid, 0 value data variant)
        emit uninstallSoftware(port);
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
