#ifndef VUBARWIDGET_H
#define VUBARWIDGET_H

#include <QWidget>

class VUBarWidget : public QWidget
{  
  Q_OBJECT

  Q_PROPERTY(quint32 numBars READ numBars WRITE setNumBars)

public:
  VUBarWidget(QWidget *parent = 0);

  void setValue(quint32 val)                      { m_currVal = val; update(); }
  quint32 value()                                 { return m_currVal; }
  void setRange(quint32 max)                      { m_maxVal = max; update(); }
  void setNumBars(quint32 numBars)                { m_numBars = numBars; update(); }
  quint32 numBars()                               { return m_numBars; }
  void setBlockSeperatorWidth(quint32 width)      { m_blockSeperatorWidth = width; update(); }
  quint32 blockSeperatorWidth()                   { return m_blockSeperatorWidth; }

signals:

public slots:

protected:

  void paintEvent(QPaintEvent* event);

private:

  quint32       m_maxVal;
  quint32       m_currVal;
  quint32       m_numBars;
  quint32       m_blockSeperatorWidth;

};

#endif // VUBARWIDGET_H
