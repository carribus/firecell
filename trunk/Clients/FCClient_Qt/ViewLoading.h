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
#ifndef _VIEWLOADING_H_
#define _VIEWLOADING_H_

#include "ui_ViewLoading.h"
#include "FCApp.h"
#include "ViewBase.h"

class ViewLoading : public ViewBase
{
  Q_OBJECT

public:
  ViewLoading(QWidget* parent);
  ~ViewLoading(void);

  /*
   * ViewBase implementation
   */
  void setupView();

signals:

  void attemptLogin(QString username, QString password);

protected slots:

  void onAppStateChanged(FCApp::StateInfo state, FCApp::StateInfo oldState);
  void onServerInfoReceived(unsigned char verMajor, unsigned char verMinor);

private:

  void addLogItem(QString item);
  void handleSubStateChange(FCApp::StateInfo state);
  void openLoginDialog();

  Ui::ViewLoading ui;

};

#endif//_VIEWLOADING_H_