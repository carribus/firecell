#include "HealthMeterWidget.h"

HealthMeterWidget::HealthMeterWidget(QWidget* parent)
: QWidget(parent)
, m_maxVal(100)
, m_currVal(0)
{
  ui.setupUi(this);
  ui.VUBar->setBlockSeperatorWidth(3);
  setNumBars(20);

  updateUI();
}

///////////////////////////////////////////////////////////////////////

HealthMeterWidget::~HealthMeterWidget(void)
{
}

///////////////////////////////////////////////////////////////////////

void HealthMeterWidget::setNumBars(quint32 num)
{
  ui.VUBar->setNumBars(num);
}

///////////////////////////////////////////////////////////////////////

void HealthMeterWidget::setMaxValue(quint32 val)
{
  m_maxVal = val;
  updateUI();
}

///////////////////////////////////////////////////////////////////////

void HealthMeterWidget::setCurrentValue(quint32 val)
{
  m_currVal = val;
  updateUI();
}

///////////////////////////////////////////////////////////////////////

void HealthMeterWidget::updateUI()
{
  QString str = QString("%1 / %2").arg(m_currVal).arg(m_maxVal);
  ui.lblValues->setText(str);
  ui.VUBar->setRange(m_maxVal);
  ui.VUBar->setValue(m_currVal);
}