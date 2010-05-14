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
#include "clientstrings.h"
#include "ViewGame.h"
#include "FCApp.h"
#include "FCMainWindow.h"

#define APPBAR_HEIGHT   25

ViewGame::ViewGame(QWidget* parent)
: ViewBase(parent)
, m_appBar(NULL)
{
  setAttribute(Qt::WA_OpaquePaintEvent);
  if ( !m_background.load(FCAPP->getResourceFolder() + "desktop.jpg") )
  {
    qDebug() << "Failed to load " + FCAPP->getResourceFolder() + "desktop.jpg";
  }
}

///////////////////////////////////////////////////////////////////////

ViewGame::~ViewGame(void)
{
}

///////////////////////////////////////////////////////////////////////

void ViewGame::setupView()
{
  // create the app bar
  m_appBar = new DesktopAppBar(this);
  m_appBar->setGeometry(0, 0, width(), APPBAR_HEIGHT);
  m_appBar->show();

  QObject::connect( m_appBar, SIGNAL(appBarOptionClicked(FCULONG)), this, SLOT(onAppBarOptionClicked(FCULONG)));
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
#include "VUBarWidget.h"
void ViewGame::onOpenSoftwareMgr()
{
  qDebug() << "onOpenSoftwareMgr()";

  VUBarWidget* bar = new VUBarWidget(this);

  bar->setGeometry(100,100,300,120);
  bar->show();
  bar->setBlockSeperatorWidth(5);
  bar->setNumBars(10);
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

  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_SOFTWAREMGR), this, SLOT(onOpenSoftwareMgr()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_ITEMMGR), this, SLOT(onOpenItemMgr()) );
  menu.addSeparator();
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_CHARINFO), this, SLOT(onOpenCharacterInfo()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_SYSTEMINFO), this, SLOT(onOpenSystemInfo()) );
  menu.addSeparator();
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_ABOUT), this, SLOT(onAbout()) );
  menu.addAction( ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM_MENU_EXIT), this, SLOT(onQuit()) );

  menu.exec(QPoint(cRect.left(), cRect.top() + APPBAR_HEIGHT));
}
