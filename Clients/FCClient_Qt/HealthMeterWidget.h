#ifndef _HEALTHMETERWIDGET_H_
#define _HEALTHMETERWIDGET_H_

#include <QWidget>
#include "ui_HealthMeterWidget.h"

class HealthMeterWidget : public QWidget
{
  Q_OBJECT
  Q_PROPERTY(quint32 numBars READ numBars WRITE setNumBars)

public:
  HealthMeterWidget(QWidget* parent);
  ~HealthMeterWidget(void);

  void setMaxValue(quint32 val);
  void setCurrentValue(quint32 val);
  void setNumBars(quint32 num);
  quint32 getMaxValue()                         { return m_maxVal; }
  quint32 getCurrentValue()                     { return m_currVal; }
  quint32 numBars()                             { return ui.VUBar->numBars(); }

protected slots:

  void updateUI();

private:

  Ui::HealthMeter ui;
  quint32 m_maxVal;
  quint32 m_currVal;

};

#endif//_HEALTHMETERWIDGET_H_