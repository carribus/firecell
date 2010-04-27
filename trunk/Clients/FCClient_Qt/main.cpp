#include "fcmainwindow.h"
#include "Settings.h"
#include <QtGui/QApplication>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
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
    QDesktopWidget* pDesktop = a.desktop();
    QRect screenRes = pDesktop->screenGeometry();
    QRect wndRect(0, 0, width, height);

    wndRect.moveCenter( screenRes.center() );
    w.setGeometry(wndRect);
    
    w.show();
  }
  return a.exec();
}
