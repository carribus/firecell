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
#ifndef FCCLIENT_QT_H
#define FCCLIENT_QT_H

#include <QtGui/QMainWindow>
#include "ui_fcmainwindow.h"
#include "fcclient_consts.h"
#include "FCApp.h"
#include "ViewBase.h"

class FCMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  FCMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~FCMainWindow();

signals:
  /*
   * This signal is here to enforce a sequence of handling this event. Views should connect to this signal and NOT the signal from FCModel
   */
//  void modelStateChanged(FCModel::e_ModelState newState, FCModel::e_ModelState oldState);

protected slots:

  void onAppStateChanged(FCApp::StateInfo state, FCApp::StateInfo oldState);
//  void onModelStateChanged(FCModel::e_ModelState newState, FCModel::e_ModelState oldState);

protected:
  void resizeEvent(QResizeEvent* event);
//  void paintEvent(QPaintEvent* event);


private:

  bool switchView(ViewBase* pNewView);

  Ui::FCMainWindow      ui;
  ViewBase*             m_currentView;
//  QPixmap m_background;
};

#endif // FCCLIENT_QT_H
