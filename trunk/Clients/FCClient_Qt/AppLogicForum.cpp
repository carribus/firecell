#include "dataobjects\Character.h"
#include "FCApp.h"
#include "FCPlayerModel.h"
#include "AppLogicForum.h"

AppLogicForum::AppLogicForum(QObject *parent)
: QObject(parent)
, m_model(NULL)
, m_view(NULL)
{
  m_model = FCAPP->forumModel();
}

///////////////////////////////////////////////////////////////////////

AppLogicForum::~AppLogicForum()
{

}

///////////////////////////////////////////////////////////////////////

void AppLogicForum::create(QWidget* parent)
{
/*
  m_view = new QTreeView(parent);
  m_view->setModel(m_model);
  m_view->setItemDelegate( new DelegateForumCategories(this) );
  m_view->resizeColumnToContents(1);
  m_view->show();
*/
  m_view = new ForumView(parent);
  m_view->setModel(m_model);
  m_view->show();
  // send a request for forum information
  FCAPP->network().sendForumCategoriesRequest( FCAPP->playerModel()->getCurrentCharacter()->GetID() );
}

///////////////////////////////////////////////////////////////////////

void AppLogicForum::destroy()
{
}

///////////////////////////////////////////////////////////////////////

QWidget* AppLogicForum::getWidget()
{
  return m_view;
}
