#include <QPushButton>
#include <QVBoxLayout>
#include "AppWindow.h"

AppWindow::AppWindow(QWidget *parent)
: QWidget(parent)
{
  QVBoxLayout* pL = new QVBoxLayout;
  setLayout(pL);
}

AppWindow::~AppWindow()
{

}
