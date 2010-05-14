#ifndef VUBARWIDGET_H
#define VUBARWIDGET_H

#include <QWidget>

class VUBarWidget : public QWidget
{  
  Q_OBJECT

public:
  VUBarWidget(QWidget *parent = 0);

  void setNumBars(quint32 numBars);
  quint32 numBars()                               { return m_numBars; }
  void setBlockSeperatorWidth(quint32 width);
  quint32 blockSeperatorWidth()                   { return m_blockSeperatorWidth; }

signals:

public slots:

protected:

  void paintEvent(QPaintEvent* event);

private:

  quint32       m_numBars;
  quint32       m_blockSeperatorWidth;

};

#endif // VUBARWIDGET_H
