#ifndef FCCLIENT_QT_H
#define FCCLIENT_QT_H

#include <QtGui/QMainWindow>
#include "ui_fcmainwindow.h"

class FCMainWindow : public QMainWindow
{
  Q_OBJECT

public:
  FCMainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
  ~FCMainWindow();

protected:
  void paintEvent(QPaintEvent* event);


private:
  Ui::FCMainWindow ui;
  QPixmap m_background;
};

#endif // FCCLIENT_QT_H
