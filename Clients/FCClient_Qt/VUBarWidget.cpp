#include <QDebug>
#include <QPainter>
#include "VUBarWidget.h"

VUBarWidget::VUBarWidget(QWidget *parent)
: QWidget(parent)
, m_maxVal(100)
, m_currVal(0)
, m_numBars(10)
, m_blockSeperatorWidth(10)
{
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QPainter painter(this); 
  QRect area = rect();
  QRectF barRect = area;
  qreal availableSpace = area.width() - ( (m_numBars-1) * m_blockSeperatorWidth);
  qreal barWidth = availableSpace / (m_numBars);
  quint32 numActiveBars = m_numBars * ((qreal)m_currVal / (qreal)m_maxVal);

  if ( barWidth > 0 )
  {
    barRect.setWidth( barWidth );
    for ( quint32 i = 0; i < m_numBars; i++ )
    {
      if ( i < numActiveBars )
        painter.fillRect(barRect, QColor(0, 255, 0));
      else
        painter.fillRect(barRect, QColor(64, 64, 64));
      barRect.setLeft( barRect.left() + barWidth + m_blockSeperatorWidth);
      barRect.setRight( barRect.right() + barWidth + m_blockSeperatorWidth);
    }
  }
}
