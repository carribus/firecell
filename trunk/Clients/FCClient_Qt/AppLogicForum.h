#ifndef APPLOGICFORUM_H
#define APPLOGICFORUM_H

#include <QObject>
#include <QTreeView>
#include "FCForumModel.h"
#include "ForumView.h"
#include "IGameAppLogic.h"

class AppLogicForum : public QObject
                    , public IGameAppLogic
{
  Q_OBJECT

public:
  AppLogicForum(QObject *parent);
  ~AppLogicForum();

  //
  // IGameAppLogic implementation
  virtual void create(QWidget* parent);
  virtual void destroy();
  virtual QWidget* getWidget();

protected slots:

  void ForumView_onForumCategoryOpened(const ForumCategory& cat);

private:

  FCForumModel*       m_model;
  ForumView*          m_view;
    
};

#endif // APPLOGICFORUM_H
