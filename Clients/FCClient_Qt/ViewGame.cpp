/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifdef _USE_STDAFX_H_
  #include "stdafx.h"
#endif//_USE_STDAFX_H_
#include <QMenu>
#include <QPainter>
#include <QMessageBox>
#include "clientstrings.h"
#include "ViewGame.h"
#include "FCApp.h"
#include "FCMainWindow.h"
#include "FCPlayerModel.h"
#include "ItemMgr.h"
#include "../../common/game_objects/ItemType.h"

#define APPBAR_HEIGHT   25

ViewGame::ViewGame(QWidget* parent)
: ViewBase(parent)
, m_appBar(NULL)
, m_pDlgSoftwareMgr(NULL)
, m_windowMgr(NULL)
{
  setAttribute(Qt::WA_OpaquePaintEvent);
  if ( !m_background.load(FCAPP->getResourceFolder() + "desktop.jpg") )
  {
    qDebug() << "Failed to load " + FCAPP->getResourceFolder() + "desktop.jpg";
  }

  m_windowMgr = new AppWindowMgr(this);
}

///////////////////////////////////////////////////////////////////////

ViewGame::~ViewGame(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewGame::setupView()
{
  FCPlayerModel* player = FCAPP->playerModel();

  // create the app bar
  m_appBar = new DesktopAppBar(this);
  m_appBar->setGeometry(0, 0, width(), APPBAR_HEIGHT);
  m_appBar->show();

  connect(m_appBar, SIGNAL(appBarOptionClicked(FCULONG)),                     this,     SLOT(onAppBarOptionClicked(FCULONG)));

  connect(player,   SIGNAL(softwareApplicationAdded(FCULONG)),                this,     SLOT(onSoftwareApplicationAdded(FCULONG)));
  connect(player,   SIGNAL(softwareActivationSucceeded(FCULONG)),             this,     SLOT(onSoftwareActivationSucceeded(FCULONG)));
  connect(player,   SIGNAL(softwareActivationFailed(FCULONG, FCULONG)),       this,     SLOT(onSoftwareActivationFailed(FCULONG, FCULONG)));

  connect(this,     SIGNAL(softwareApplicationActivated(FCULONG)),            player,   SLOT(onSoftwareApplicationActivated(FCULONG)));
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onAppBarOptionClicked(FCULONG id)
{
  if ( 0 == id )      // System menu item
  {
    // Popup the System menu
    showSystemMenu();
  }
  else                // app specific menu item
  {
  }
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onSoftwareApplicationAdded(FCULONG itemID)
{
  qDebug() << "onSoftwareApplicationAdded(" << itemID << ")";
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onSoftwareActivationSucceeded(FCULONG itemID)
{
  qDebug() << "onSoftwareActivationSucceeded(" << itemID << ")";
  m_windowMgr->openApplicationWindow(itemID, this);
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onSoftwareActivationFailed(FCULONG itemID, FCULONG result)
{
  QString strError;
  QString strTemp;
  QString strID = QString("ITEM_SOFTWARE_%1").arg(itemID);

  // prepare the error message  
  strError = QString( FETCH_STRING( ERROR_SWACTIVATE_INTRO ) )
                      .arg( ResourceManager::instance().getClientString( strID ) );

  switch ( result )
  {
  case  ACTIVATERESULT_NOT_ENOUGH_MEM:
    strTemp = FETCH_STRING( ERROR_SWACTIVATE_MEM );
    break;

  case  ACTIVATERESULT_NOT_ENOUGH_CPU:
    strTemp = FETCH_STRING( ERROR_SWACTIVATE_CPU );
    break;

  case  ACTIVATERESULT_NEED_HTTP:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_20 ) );
    break;

  case  ACTIVATERESULT_NEED_FTP:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_21 ) );
    break;

  case  ACTIVATERESULT_NEED_SSH:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_22 ) );
    break;

  case  ACTIVATERESULT_NEED_BANK:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_23 ) );
    break;

  case  ACTIVATERESULT_NEED_DB:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_24 ) );
    break;

  case  ACTIVATERESULT_NEED_MAIL:
    strTemp = QString( FETCH_STRING( ERROR_SWACTIVATE_SERVICE_MISSING ) )
                       .arg( FETCH_STRING( ITEM_SOFTWARE_25 ) );
    break;

  default:
    break;
  }

  strError += strTemp;

  QMessageBox::warning(this,
                       FETCH_STRING( ERROR_SWACTIVATE_TITLE ),
                       strError);
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onOpenSoftwareMgr()
{
  qDebug() << "onOpenSoftwareMgr()";
  
  if ( !m_pDlgSoftwareMgr )
    m_pDlgSoftwareMgr = new DlgSoftwareMgr(this);

  if ( m_pDlgSoftwareMgr )
    m_pDlgSoftwareMgr->show();
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onOpenItemMgr()
{
  qDebug() << "onOpenItemMgr()";
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onOpenCharacterInfo()
{
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onOpenSystemInfo()
{
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onAbout()
{
}

///////////////////////////////////////////////////////////////////////

void ViewGame::onQuit()
{
  FCAPP->quit();
}

///////////////////////////////////////////////////////////////////////

void ViewGame::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QPainter painter(this);
  QRectF dest(0.0, 0.0, width(), height());
  QRectF source(0.0, 0.0, m_background.width(), m_background.height());

  if ( !m_background.data_ptr() )
    painter.fillRect( dest, QColor( 0, 0, 0 ) );
  else
    painter.drawPixmap(dest, m_background, source);
}

///////////////////////////////////////////////////////////////////////

void ViewGame::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event);
  m_appBar->setGeometry(0, 0, width(), APPBAR_HEIGHT);
}

///////////////////////////////////////////////////////////////////////

void ViewGame::showSystemMenu()
{
  QRect cRect = FCAPP->mainWindow()->geometry();
  QMenu menu(this);

  QMenu* pSubMenu = menu.addMenu( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_APPLICATIONS) );
  menu.addSeparator();
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_SOFTWAREMGR), this, SLOT(onOpenSoftwareMgr()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_ITEMMGR), this, SLOT(onOpenItemMgr()) );
  menu.addSeparator();
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_CHARINFO), this, SLOT(onOpenCharacterInfo()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_SYSTEMINFO), this, SLOT(onOpenSystemInfo()) );
  menu.addSeparator();
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_ABOUT), this, SLOT(onAbout()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_EXIT), this, SLOT(onQuit()) );

  // add the applications to the applications sub-menu
  FCPlayerModel* player = FCAPP->playerModel();
  ItemMgr& itemMgr = player->itemMgr();
  std::vector<ItemMgr::GameItem> items;
  Item* pItem = NULL;
  ItemSoftware* pSoftware = NULL;

  itemMgr.getItems(items);

  std::vector<ItemMgr::GameItem>::iterator it = items.begin();
  std::vector<ItemMgr::GameItem>::iterator limit = items.end();

  for ( ; it != limit; ++it )
  {
    pItem = it->getItem();
    if ( pItem && pItem->GetTypeID() == ItemType::Software )
    {
      pSoftware = static_cast<ItemSoftware*>(pItem);
      if ( !pSoftware->IsService() && pSoftware->GetDesktopIconFlag() )
      {
        QAction* pAction = pSubMenu->addAction( QString("%1").arg(pItem->GetName().c_str()) );
        QVariant v( (qulonglong) pItem->GetID() );
        pAction->setData(v);
      }
    }
  }

  // show the menu
  QAction* pSelAction = menu.exec(QPoint(cRect.left(), cRect.top() + APPBAR_HEIGHT));

  // check what the selection was, and if the user chose to launch an application, deal with it
  if ( pSelAction )
  {
    QVariant v = pSelAction->data();

    if ( v.type() != QVariant::Invalid )
    {
      FCULONG itemID = v.value<FCULONG>();

      emit softwareApplicationActivated(itemID);
    }
  }
}
