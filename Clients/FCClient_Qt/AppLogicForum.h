#ifndef APPLOGICFORUM_H
#define APPLOGICFORUM_H

#include <QObject>
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
#error You need to create the following two classes
  FCForumModel*       m_model;
  ForumView*          m_view;
    
};

#endif // APPLOGICFORUM_H
