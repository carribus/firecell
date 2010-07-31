#ifndef APPLOGICFORUM_H
#define APPLOGICFORUM_H

#include <QObject>
#include <QTreeView>
#include "FCForumModel.h"
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

private:

  FCForumModel*       m_model;
  QTreeView*          m_view;
//  ForumView*          m_view;
    
};

#endif // APPLOGICFORUM_H
