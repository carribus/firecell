#include "DelegateForumCategories.h"
#include "ForumView.h"

ForumView::ForumView(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  ui.treeView->setItemDelegate( new DelegateForumCategories(this) );
}

///////////////////////////////////////////////////////////////////////

ForumView::~ForumView()
{

}

///////////////////////////////////////////////////////////////////////

void ForumView::setModel(QAbstractItemModel* pModel)
{
  ui.treeView->setModel(pModel);
  ui.treeView->setColumnWidth(0, 450);
  ui.treeView->setColumnWidth(1, ui.treeView->width()-450);
}