#ifndef _FORUMCATEGORY_H_
#define _FORUMCATEGORY_H_

#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "../common/PThreadMutex.h"
#include "ForumPost.h"

using namespace std;

class ForumCategory
{
public:
	ForumCategory()
		: m_id(0), m_parentID(0), m_accountTypeRequired(0), m_minLevel(0), m_maxLevel(0)
    , m_mutexPosts(true)
	{
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

	~ForumCategory()
	{
    m_mutexPosts.Lock();
		PostsVector::iterator it = m_posts.begin();

		while ( it != m_posts.end() )
		{
			delete (*it);
			it++;
		}
    m_mutexPosts.Unlock();
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

	size_t AddForumPost(ForumPost* pPost)
	{
		if ( !pPost )
			return 0;

		size_t result = 0;

		if ( pPost->GetParentID() == 0 )
		{
      m_mutexPosts.Lock();
			m_posts.push_back(pPost);
			result = m_posts.size();
      m_mutexPosts.Unlock();
		}
		else
		{
			ForumPost* pParent = GetPostByID(pPost->GetParentID());

			if ( pParent )
			{
				result = pParent->AddForumPost(pPost);
			}
		}

		return result;
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

	ForumPost* GetPostByID(FCULONG id)
	{
    m_mutexPosts.Lock();
		vector<ForumPost*>::iterator it = m_posts.begin();

		while ( it != m_posts.end() )
		{
			if ( (*it)->GetID() == id )
      {
        m_mutexPosts.Unlock();
				return (*it);
      }
		}

    m_mutexPosts.Unlock();

		return NULL;
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

	size_t GetForumPosts(vector<ForumPost>& target)
	{
    m_mutexPosts.Lock();
		PostsVector::iterator it = m_posts.begin();
		ForumPost* pPost = NULL;

		while ( it != m_posts.end() )
		{
			pPost = (*it);
			target.push_back(*pPost);
			it++;
		}

    m_mutexPosts.Unlock();
		return target.size();
	}

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

	FCULONG						GetID()												{ return m_id; }
	FCULONG						GetParentID()									{ return m_parentID; }
	FCULONG						GetOrder()										{ return m_order; }
	string						GetName()											{ return m_name; }
	string						GetDescription()							{ return m_description; }
	FCULONG						GetAccountTypeRequired()			{ return m_accountTypeRequired; }
	FCULONG						GetMinLevel()									{ return m_minLevel; }
	FCULONG						GetMaxLevel()									{ return m_maxLevel; }
  size_t            GetPostCount()                { return m_posts.size(); }

	void							SetID(FCULONG id)							{ m_id = id; }
	void							SetParentID(FCULONG id)				{ m_parentID = id; }
	void							SetOrder(FCULONG order)				{ m_order = order; }
	void							SetName(string name)					{ m_name = name; }
	void							SetDescription(string desc)		{ m_description = desc; }
	void							SetAccountTypeRequired(FCULONG type)		{ m_accountTypeRequired = type; }
	void							SetMinLevel(FCULONG lvl)			{ m_minLevel = lvl; }
	void							SetMaxLevel(FCULONG lvl)			{ m_maxLevel = lvl; }

private:

	FCULONG						m_id;
	FCULONG						m_parentID;
	FCULONG						m_order;
	string						m_name;
	string						m_description;
	FCULONG						m_accountTypeRequired;
	FCULONG						m_minLevel;
	FCULONG						m_maxLevel;

  PThreadMutex      m_mutexPosts;
	typedef vector<ForumPost*> PostsVector;
	PostsVector				m_posts;
};

#endif//_FORUMCATEGORY_H_