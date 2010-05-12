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
#include <QMouseEvent>
#include <QPainter>
#include "clientstrings.h"
#include "desktopappbar.h"
#include "ResourceManager.h"

#define APPBAR_ITEM_PADDING_X 30

DesktopAppBar::DesktopAppBar(QWidget *parent) : QWidget(parent)
{
  setMouseTracking(true);

  m_fontItems = new QFont("Arial", 13, QFont::Normal, false);
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

DesktopAppBar::~DesktopAppBar()
{
  if ( m_fontItems )
    delete m_fontItems;
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::addMenuItem(QString itemText)
{
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  QRectF area(0, 0, width(), height());

  drawBackground(painter, area);
  drawSystemMenuItem(painter, area);
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

void DesktopAppBar::drawSystemMenuItem(QPainter& painter, QRectF area)
{
  QPen pen( QColor(64,64,64,194) );
  QPen pen2( QColor(255,255,255,64));
  AppBarOption abo = m_appBarOptions[0];
  QRectF r = abo.rect;

  if ( !m_fontItems )
    return;

  painter.setFont(*m_fontItems);
  abo.rect.setBottom(area.bottom());

  // shade the background
  painter.fillRect(r, abo.bHighlight ? QColor(0, 0, 194, 128) : QColor(0, 0, 0, 128));

  // draw the seperator
  painter.setPen(pen);
  painter.drawLine( r.right()-1, r.top(), r.right()-1, r.bottom() );
  painter.setPen(pen2);
  painter.drawLine( r.right(), r.top(), r.right(), r.bottom() );

  pen.setColor(QColor(255,255,255, abo.bHighlight ? 255 : 128));
  painter.setPen(pen);
  painter.drawText( abo.rect, Qt::AlignHCenter | Qt::AlignVCenter, abo.str );
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::drawMenuItems(QPainter& painter, QRectF area)
{
  AppBarOptionVector::iterator it = m_appBarOptions.begin();
  AppBarOptionVector::iterator limit = m_appBarOptions.end();
  AppBarOption abo;

  for ( ; it != limit; ++it )
  {
    abo = *it;
    if ( !abo.bAppOption )
    {

    }
  }
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::mouseMoveEvent(QMouseEvent* event)
{
  QPoint mousePt = event->pos();
  QRect area = this->geometry();

  if ( area.contains(mousePt) )
  {
    AppBarOptionVector::iterator it = m_appBarOptions.begin();
    AppBarOptionVector::iterator limit = m_appBarOptions.end();

    for ( ; it != limit; ++it )
    {
      (*it).bHighlight = (*it).rect.contains(mousePt);
    }
    this->update();
  }
}

///////////////////////////////////////////////////////////////////////

void DesktopAppBar::leaveEvent(QEvent* event)
{
  AppBarOptionVector::iterator it = m_appBarOptions.begin();
  AppBarOptionVector::iterator limit = m_appBarOptions.end();

  for ( ; it != limit; ++it )
  {
    (*it).bHighlight = false;
  }
  this->update();
}
