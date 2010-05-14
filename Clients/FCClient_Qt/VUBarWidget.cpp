#include <QDebug>
#include <QPainter>
#include "VUBarWidget.h"

VUBarWidget::VUBarWidget(QWidget *parent)
: QWidget(parent)
, m_numBars(10)
, m_blockSeperatorWidth(10)
{
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::setNumBars(quint32 numBars)
{
  m_numBars = numBars;
  update();
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::setBlockSeperatorWidth(quint32 width)
{
  m_blockSeperatorWidth = width;
  update();
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QPainter painter(this); 
  QRect area = geometry();
  QRect barRect = area;
  quint32 availableSpace = area.width() - ( (m_numBars) * m_blockSeperatorWidth);
  quint32 barWidth = availableSpace / (m_numBars);

  qDebug() << "\nVUBar Area: " << area.left() << "," << area.top() << "," << area.right() << "," << area.bottom();
  painter.fillRect(area, QColor(0, 0, 0));
  if ( barWidth > 0 )
  {
    barRect.setWidth( barWidth );
    for ( quint32 i = 0; i < m_numBars; i++ )
    {
      qDebug() << i << ": " << barRect;
      painter.fillRect(barRect, QColor(0, 255, 0));
      barRect.setLeft( barRect.right() + m_blockSeperatorWidth);
      barRect.setRight( barRect.left() + barWidth );
    }
  }
}
