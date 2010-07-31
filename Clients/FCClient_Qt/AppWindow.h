#ifndef APPWINDOW_H
#define APPWINDOW_H

#include <QWidget>

class AppWindow : public QWidget
{
  Q_OBJECT

public:
  AppWindow(QWidget *parent);
  ~AppWindow();

};

#endif // APPWINDOW_H
