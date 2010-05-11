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
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include <QFont>
#include <QPainter>
#include "clientstrings.h"
#include "desktopappbar.h"
#include "ResourceManager.h"

#define APPBAR_ITEM_PADDING_X 30

DesktopAppBar::DesktopAppBar(QWidget *parent) : QWidget(parent)
{
  m_fontItems = new QFont("Arial", 10, QFont::Normal, false);
  QFontMetrics fm(*m_fontItems, NULL);

  // create the default AppBar System Menu Item
  AppBarOption abo;

  abo.id = 0;
  abo.bAppOption = false;
  abo.bHighlight = false;
  abo.str = ResourceManager::instance().getClientString( STR_APP_APPBAR_SYSTEM );
  abo.rect = geometry();
  abo.rect.setRight( abo.rect.left() + fm.width( abo.str ) + APPBAR_ITEM_PADDING_X );

  m_appBarOptions.push_back(abo);
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::addMenuItem(QString itemText)
{
  if ( m_fontItems )
    delete m_fontItems;
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  QRectF area(0, 0, width(), height());

  drawBackground(painter, area);
  drawMenuItems(painter, area);
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawBackground(QPainter& painter, QRectF area)
{
  QLinearGradient gradient(0, 0, 0, height());

  gradient.setColorAt(0, QColor(255, 255, 255, 196));
  gradient.setColorAt(1, QColor(64, 64, 64, 196));

  QBrush brush(gradient);
  painter.fillRect(area, brush);
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawMenuItems(QPainter& painter, QRectF area)
{
  // TODO: Render the menu items
}
