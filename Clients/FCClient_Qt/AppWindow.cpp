#include <QPushButton>
#include <QVBoxLayout>
#include "AppWindow.h"

AppWindow::AppWindow(QWidget *parent)
: QWidget(parent)
{
  QVBoxLayout* pL = new QVBoxLayout;
  pL->setMargin(0);
  setLayout(pL);
}

AppWindow::~AppWindow()
{

}
