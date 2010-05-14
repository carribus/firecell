#include <QPainter>
#include "VUBarWidget.h"

VUBarWidget::VUBarWidget(QWidget *parent)
: QWidget(parent)
, m_numBars(10)
, m_blockSeperatorWidth(5)
{
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::setNumBars(quint32 numBars)
{
  m_numBars = numBars;
  update();
}

///////////////////////////////////////////////////////////////////////

void VUBarWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event);

  QPainter painter(this);
  QRect area = geometry();
  QRect barRect = area;
  qreal availableSpace = area.width() - ((m_numBars-1) * m_blockSeperatorWidth);
  qreal barWidth = availableSpace / (m_numBars*2);

  painter.fillRect(area, QColor(0, 0, 0));
  if ( barWidth > 0 )
  {
    barRect.setWidth( barWidth );
    for ( quint32 i = 0; i < m_numBars; i++ )
    {
      painter.fillRect(barRect, QColor(0, 255, 0));
      barRect.setLeft( barRect.right() + m_blockSeperatorWidth-1);
      barRect.setRight( barRect.left() + barWidth );
    }
  }
}
