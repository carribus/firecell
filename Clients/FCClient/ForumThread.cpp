#include "ForumThread.h"

ForumThread::ForumThread(void)
: m_category_id(0)
, m_thread_id(0)
, m_parent_id(0)
, m_order(0)
, m_author_id(0)
, m_mission_id(0)
, m_contentIndex(0)
, m_contentLen(0)
{
}

///////////////////////////////////////////////////////////////////////

ForumThread::ForumThread(FCULONG categoryID, FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID)
: m_category_id(categoryID)
, m_thread_id(threadID)
, m_parent_id(parentID)
, m_order(order)
, m_title(title)
, m_author_id(authorID)
, m_author_name(authorName)
, m_date_created(dateCreated)
, m_mission_id(missionID)
{
}

///////////////////////////////////////////////////////////////////////

ForumThread::~ForumThread(void)
{
}

///////////////////////////////////////////////////////////////////////

ForumThread* ForumThread::addPost(FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID)
{
  if ( parentID != m_thread_id )
    return NULL;

  ForumThread* pThread = new ForumThread(m_category_id, threadID, parentID, order, title, authorID, authorName, dateCreated, missionID);

  if ( pThread )
    m_posts.push_back(pThread);

  return pThread;
}

///////////////////////////////////////////////////////////////////////

ForumThread* ForumThread::getPost(FCULONG threadID)
{
  if ( threadID == 0 )
    return NULL;

  ThreadVector::iterator it = m_posts.begin();
  ThreadVector::iterator limit = m_posts.end();

  for ( ; it != limit; it++ )
  {
    if ( (*it)->getThreadID() == threadID )
      return (*it);
  }

  return NULL;
}