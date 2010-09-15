#include "dataobjects/Character.h"
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
  m_view = new ForumView(parent);
  m_view->setModel(m_model);

  connect(m_view, SIGNAL(forumCategoryOpened(const ForumCategory&)), this, SLOT(ForumView_onForumCategoryOpened(const ForumCategory&)));

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

///////////////////////////////////////////////////////////////////////

void AppLogicForum::ForumView_onForumCategoryOpened(const ForumCategory& cat)
{
  FCAPP->network().sendForumThreadsRequest(cat.getID());
  m_view->switchView(ForumView::ViewForumThread);
}
