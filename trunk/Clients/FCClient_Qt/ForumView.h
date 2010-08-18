#ifndef FORUMVIEW_H
#define FORUMVIEW_H

#include <QWidget>
#include "ui_ForumView.h"
#include "ForumCategory.h"

class ForumView : public QWidget
{
    Q_OBJECT

public:

  enum ForumViewType
  {
    ViewForumCategories = 0,
    ViewForumThreadList = 1,
    ViewForumThread = 2
  };

  ForumView(QWidget *parent = 0);
  ~ForumView();

  void setModel(QAbstractItemModel* pModel);
  void switchView(ForumViewType type);

signals:
  void forumCategoryOpened(const ForumCategory& cat);

protected slots:

  void onForumCategoryDoubleClicked(const QModelIndex& index);

private:
  Ui::ForumViewClass ui;
  ForumViewType m_curViewType;
};

#endif // FORUMVIEW_H
