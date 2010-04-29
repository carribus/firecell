#ifndef _VIEWBASE_H_
#define _VIEWBASE_H_

#include <QWidget>

class ViewBase : public QWidget
{
  Q_OBJECT

public:
  ViewBase(QWidget* parent);
  virtual ~ViewBase(void);

  virtual void setupView() = 0;
};

#endif//_VIEWBASE_H_