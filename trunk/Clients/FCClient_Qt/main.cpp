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
#include "Settings.h"
#include "FCApp.h"

int main(int argc, char *argv[])
{
  FCApp app(argc, argv);

/*
  FCMainWindow w;

  Settings& settings = Settings::instance();

  settings.LoadSettings("./clientdata/settings.xml");

  int fullscreen = settings.GetValue("FCClient/Settings/Fullscreen", "useFullScreen").toInt();
  int width = settings.GetValue("FCClient/Settings/Resolution", "width").toInt();
  int height = settings.GetValue("FCClient/Settings/Resolution", "height").toInt();

  if ( fullscreen )
  {
    w.showFullScreen();
  }
  else
  {
    QDesktopWidget* pDesktop = app.desktop();
    QRect screenRes = pDesktop->screenGeometry();
    QRect wndRect(0, 0, width, height);

    wndRect.moveCenter( screenRes.center() );
    w.setGeometry(wndRect);
    
    w.show();
  }
*/

  if ( app.initialise() )
  {
    QMetaObject::invokeMethod (&app, "bootUp", Qt::QueuedConnection);
    return app.exec();
  }

  return 0;
}
