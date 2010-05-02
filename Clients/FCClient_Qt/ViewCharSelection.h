#ifndef _VIEWCHARSELECTION_H_
#define _VIEWCHARSELECTION_H_

#include "ui_ViewCharSelection.h"
#include "ViewBase.h"

class ViewCharSelection : public ViewBase
{
  Q_OBJECT

public:
  ViewCharSelection(QWidget* parent);
  ~ViewCharSelection(void);

  /*
   * ViewBase implementation
   */
  void setupView();

private:

  Ui::ViewCharSelection ui;

};

#endif//_VIEWCHARSELECTION_H_