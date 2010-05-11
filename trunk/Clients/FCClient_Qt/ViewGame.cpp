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
#include <QPainter>
#include "ViewGame.h"
#include "FCApp.h"

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
  m_appBar = new DesktopAppBar(this);
  m_appBar->setGeometry(0, 0, width(), APPBAR_HEIGHT);
  m_appBar->show();
}

///////////////////////////////////////////////////////////////////////

void ViewGame::paintEvent(QPaintEvent* event)
{
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
  m_appBar->setGeometry(0, 0, width(), APPBAR_HEIGHT);
}
