#include "../common/Logging/DynLog.h"
#include "../common/database/FCDatabase.h"
#include "EventSystem.h"
#include "Event.h"
#include "ForumPost.h"
#include "Forum.h"

DEFINE_EVENT_SOURCE(Forum);
DEFINE_EVENT(Forum, NewCategory);
DEFINE_EVENT(Forum, NewThread);
DEFINE_EVENT(Forum, NewReply);

Forum::Forum(void)
: m_mutexCategories(true)
, m_lastPostID(0)
{
}

///////////////////////////////////////////////////////////////////////

Forum::~Forum()
{
	vector<ForumCategory*>::iterator it = m_categories.begin();
	vector<ForumCategory*>::iterator limit = m_categories.end();

	m_mutexCategories.Lock();
	for ( ; it != limit; it++ )
	{
		delete *it;
	}
	m_mutexCategories.Unlock();
}

///////////////////////////////////////////////////////////////////////

size_t Forum::AddCategory(FCULONG id, FCULONG parentID, FCULONG order, string name, string desc, FCULONG accountTypeReq, FCULONG minLevel, FCULONG maxLevel)
{
	ForumCategory* pCat = GetCategoryByID(id);

	if ( !pCat )
	{
		pCat = new ForumCategory;
		pCat->SetID(id);
		pCat->SetParentID(parentID);
		pCat->SetOrder(order);
		pCat->SetName(name);
		pCat->SetDescription(desc);
		pCat->SetAccountTypeRequired(accountTypeReq);
		pCat->SetMinLevel(minLevel);
		pCat->SetMaxLevel(maxLevel);

    m_mutexCategories.Lock();
		m_categories.push_back(pCat);
    m_mutexCategories.Unlock();
	}

	return m_categories.size();
}

///////////////////////////////////////////////////////////////////////

size_t Forum::GetCategoryCount()
{
	return m_categories.size();
}

///////////////////////////////////////////////////////////////////////

size_t Forum::GetCategoriesForPlayer(const Player* pPlayer, vector<ForumCategory*>& target)
{
	if ( !pPlayer )
		return 0;

	size_t count = 0;
	ForumCategory* pCat = NULL;

  m_mutexCategories.Lock();
	// Enumerate all categories that the player can see and return them in target
	count = m_categories.size();
	for ( size_t i = 0; i < count; i++ )
	{
		pCat = m_categories.at(i);

		// TODO: Still need to test account type to make sure that the player can see the category...
		if ( pPlayer->GetLevel() >= pCat->GetMinLevel() &&
				 pPlayer->GetLevel() <= pCat->GetMaxLevel() )
		{
			target.push_back(pCat);
		}
	}
  m_mutexCategories.Unlock();

	return target.size();
}

///////////////////////////////////////////////////////////////////////

ForumCategory* Forum::GetCategoryByID(FCULONG id)
{
  m_mutexCategories.Lock();

	vector<ForumCategory*>::iterator it = m_categories.begin();

	while ( it != m_categories.end() )
	{
		if ( (*it)->GetID() == id )
    {
      m_mutexCategories.Unlock();
			return *it;
    }
		it++;
	}

  m_mutexCategories.Unlock();

	return NULL;
}

///////////////////////////////////////////////////////////////////////

bool Forum::CreateNewForumThread(FCULONG category_id, FCULONG thread_id, FCULONG author_id, std::string author_name, std::string title, std::string content)
{
  bool bResult = false;
  std::string now;
  ForumCategory* pCat = GetCategoryByID(category_id);
	ForumPost* pThread = NULL;

  if ( pCat )
  {
		// verify that the thread that this post should be childed to actually exists...
		if ( thread_id > 0 )
		{
			pThread = pCat->GetPostByID(thread_id);
			if ( !pThread )
			{
				DYNLOG_ADDLOG( DYNLOG_FORMAT("Forum::CreateNewForumThread FAILED: ThreadID %ld could not be found as parent", thread_id) );
				return false;
			}
		}

		ForumPost* pNewPost = NULL;
    time_t t = time(NULL);
		FCULONG postID = GetNewPostID();
    tm* pTime = gmtime( &t );
    char now[256];

    sprintf(now, "%ld-%02ld-%02ld %02ld:%02ld:%02ld", pTime->tm_year+1900, pTime->tm_mon+1, pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
    if ( (pNewPost = AddForumPost( postID, thread_id, category_id, (FCULONG)-1, title, content, author_id, author_name, now )) )
		{
      // save the post to the database
      FCDatabase::instance().ExecuteJob(DBQ_SAVE_FORUM_POST, NULL, postID, thread_id, category_id, pNewPost->GetOrder(), title.c_str(), author_id, content.c_str());

      // emit an event for the new forum post
      EventSystem::GetInstance()->Emit( this, NULL, new Event(Forum::EVT_NewThread, (void*)pNewPost) );

			bResult = true;
		}
  }
  else
  {
    DYNLOG_ADDLOG( DYNLOG_FORMAT("Forum::CreateNewForumThread() - could not locate category id %ld", category_id) );
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

ForumPost* Forum::AddForumPost(FCULONG id, FCULONG parentID, FCULONG category_id, FCULONG order, string title, string content, FCULONG author_id, string author_name, string date_created, FCULONG mission_id)
{
	ForumCategory* pCat = GetCategoryByID(category_id);
	ForumPost* pPost = NULL;
	size_t result = 0;

	if ( pCat )
	{
		pPost = new ForumPost;
		pPost->SetID(id);
		pPost->SetParentID(parentID);
		pPost->SetOrder(order);
		pPost->SetTitle(title);
		pPost->SetContent(content);
		pPost->SetAuthorID(author_id);
    pPost->SetAuthorName(author_name);
		pPost->SetDateCreated(date_created);
		pPost->SetMissionID(mission_id);

		result = pCat->AddForumPost(pPost);

    if ( m_lastPostID < id )
      m_lastPostID = id;
	}

	return pPost;
}

///////////////////////////////////////////////////////////////////////

size_t Forum::GetForumThreads(FCULONG category_id, vector<ForumPost*>& target)
{
	ForumCategory* pCat = GetCategoryByID(category_id);

	if ( pCat )
	{
		pCat->GetForumPosts(target);
	}

	return target.size();
}

///////////////////////////////////////////////////////////////////////

FCULONG Forum::GetNewPostID()
{
  FCULONG returnVal = 0;

  m_mutexPostID.Lock();
  returnVal = ++m_lastPostID;
  m_mutexPostID.Unlock();

  return returnVal;

}