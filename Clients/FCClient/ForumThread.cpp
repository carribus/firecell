#include "ForumThread.h"

ForumThread::ForumThread(void)
: m_thread_id(0)
, m_parent_id(0)
, m_order(0)
, m_author_id(0)
, m_mission_id(0)
{
}

///////////////////////////////////////////////////////////////////////

ForumThread::ForumThread(FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID)
: m_thread_id(threadID)
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
