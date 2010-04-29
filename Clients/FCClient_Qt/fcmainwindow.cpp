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
#include <QPainter>
#include <QDebug>
#include "fcmainwindow.h"
#include "ViewLoading.h"

FCMainWindow::FCMainWindow(QWidget *parent, Qt::WFlags flags)
: QMainWindow(parent, flags)
, m_currentView(NULL)
{
  ui.setupUi(this);
/*
  setAttribute(Qt::WA_OpaquePaintEvent);
  m_background.load("./clientdata/desktop.jpg");

  installEventFilter(this);
*/
}

///////////////////////////////////////////////////////////////////////

FCMainWindow::~FCMainWindow()
{

}

///////////////////////////////////////////////////////////////////////

void FCMainWindow::onModelStateChanged(FCModel::e_ModelState newState, FCModel::e_ModelState oldState)
{
  switch ( newState )
  {
  case  FCModel::LoadingState:
    switchView( new ViewLoading(this) );
    break;
  case  FCModel::ConnectingState:
    break;
  case  FCModel::LoginState:
    break;
  case  FCModel::CharacterSelectionState:
    break;
  case  FCModel::PlayingState:
    break;
  case  FCModel::ShuttingDownState:
    break;
  default:
    break;
  }
}

///////////////////////////////////////////////////////////////////////

void FCMainWindow::resizeEvent(QResizeEvent* event)
{
  // resize the current view to fill up the main window
  if ( m_currentView )
  {
    QRect thisRect = geometry();
    m_currentView->resize(thisRect.width(), thisRect.height());
  }
}

///////////////////////////////////////////////////////////////////////
/*
void FCMainWindow::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  QRectF dest(0.0, 0.0, width(), height());
  QRectF source(0.0, 0.0, m_background.width(), m_background.height());

  painter.drawPixmap(dest, m_background, source);
  
}
*/

///////////////////////////////////////////////////////////////////////

bool FCMainWindow::switchView(ViewBase* pNewView)
{
  if ( !pNewView )
    return false;

  qDebug() << "Switching Views";

  if ( m_currentView )
  {
    m_currentView->deleteLater();
    m_currentView->hide();
  }

  m_currentView = pNewView;

  m_currentView->setupView();

  QRect thisRect = geometry();
  m_currentView->resize(thisRect.width(), thisRect.height());

  // finally show the new view
  m_currentView->show();

  return true;
}