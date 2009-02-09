#include "ForumPost.h"
#include "Forum.h"

Forum::Forum(void)
: m_mutexCategories(true)
{
}

///////////////////////////////////////////////////////////////////////

Forum::~Forum()
{
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

size_t Forum::AddForumPost(FCULONG id, FCULONG parentID, FCULONG category_id, FCULONG order, string title, string content, FCULONG author_id, string date_created, FCULONG mission_id)
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
		pPost->SetDateCreated(date_created);
		pPost->SetMissionID(mission_id);

		result = pCat->AddForumPost(pPost);
	}

	return result;
}

///////////////////////////////////////////////////////////////////////

size_t Forum::GetForumThreads(FCULONG category_id, vector<ForumPost>& target)
{
	ForumCategory* pCat = GetCategoryByID(category_id);

	if ( pCat )
	{
		pCat->GetForumPosts(target);
	}

	return target.size();
}