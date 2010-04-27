#include <QPainter>
#include "fcmainwindow.h"

FCMainWindow::FCMainWindow(QWidget *parent, Qt::WFlags flags)
    : QMainWindow(parent, flags)
{
  ui.setupUi(this);
  setAttribute(Qt::WA_OpaquePaintEvent);
  m_background.load("./clientdata/desktop.jpg");

  installEventFilter(this);
}

FCMainWindow::~FCMainWindow()
{

}

void FCMainWindow::paintEvent(QPaintEvent* event)
{
  QPainter painter(this);
  QRectF dest(0.0, 0.0, width(), height());
  QRectF source(0.0, 0.0, m_background.width(), m_background.height());

  painter.drawPixmap(dest, m_background, source);
  
}