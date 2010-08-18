#include <QDebug>
#include "DelegateForumCategories.h"
#include "FCApp.h"
#include "ForumView.h"

ForumView::ForumView(QWidget *parent)
: QWidget(parent)
, m_curViewType(ViewForumCategories)
{
  ui.setupUi(this);

  ui.stackedWidget->setCurrentIndex(0);

  ui.treeView->setItemDelegate( new DelegateForumCategories(this) );
  connect(ui.treeView, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(onForumCategoryDoubleClicked(const QModelIndex&)));
}

///////////////////////////////////////////////////////////////////////

ForumView::~ForumView()
{
}

///////////////////////////////////////////////////////////////////////

void ForumView::setModel(QAbstractItemModel* pModel)
{
  ui.treeView->setExpandsOnDoubleClick(false);
  ui.treeView->setModel(pModel);
  ui.treeView->setColumnWidth(0, 450-1);
  ui.treeView->setColumnWidth(1, ui.treeView->width()-450-1);
}

///////////////////////////////////////////////////////////////////////

void ForumView::switchView(ForumViewType type)
{
  if ( type != m_curViewType )
  {
    switch ( type )
    {
    case  ViewForumCategories:
      break;

    case  ViewForumThreadList:
      break;

    case  ViewForumThread:
      break;

    default:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////

void ForumView::onForumCategoryDoubleClicked(const QModelIndex& index)
{
  ForumCategory* pCat = static_cast<ForumCategory*>(index.internalPointer());

  emit forumCategoryOpened(*pCat);
}