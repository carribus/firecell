#ifndef DESKTOPAPPBAR_H
#define DESKTOPAPPBAR_H

#include <QWidget>

class DesktopAppBar : public QWidget
{
  Q_OBJECT

public:
  DesktopAppBar(QWidget *parent = 0);

signals:

public slots:

protected:

  void paintEvent(QPaintEvent* event);
    void drawBackground(QPainter& painter, QRectF area);
    void drawMenuItems(QPainter& painter, QRectF area);

private:


};

#endif // DESKTOPAPPBAR_H
