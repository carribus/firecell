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
#include <QPainter>
#include "desktopappbar.h"

DesktopAppBar::DesktopAppBar(QWidget *parent) : QWidget(parent)
{
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
}
