#ifndef _FORUMPOST_H_
#define _FORUMPOST_H_

#include <string>
#include <vector>
#include "../common/fctypes.h"

using namespace std;

class ForumPost
{
public:

	ForumPost() : m_id(0), m_parentID(0), m_order(0), m_authorID(0), m_missionID(0), m_locked(false) {}
	
	~ForumPost() 
	{
		vector<ForumPost*>::iterator it = m_posts.begin();

		while ( it != m_posts.end() )
		{
			delete (*it);
			it++;
		}
	}

	size_t AddForumPost(ForumPost* pPost)
	{
		m_posts.push_back(pPost);
		return m_posts.size();
	}

	FCULONG				GetID()															{ return m_id; }
	FCULONG				GetParentID()												{ return m_parentID; }
	FCULONG				GetOrder()													{ return m_order; }
	string				GetTitle()													{ return m_title; }
	string				GetContent()												{ return m_content; }
	FCULONG				GetAuthorID()												{ return m_authorID; }
	string				GetAuthorName()											{ return m_author_name; }
	string				GetDateCreated()										{ return m_dateCreated; }
	FCULONG				GetMissionID()											{ return m_missionID; }
	bool					IsLocked()													{ return m_locked; }

	void					SetID(FCULONG id)										{ m_id = id; }
	void					SetParentID(FCULONG id)							{ m_parentID = id; }
	void					SetOrder(FCULONG order)							{ m_order = order; }
	void					SetTitle(string title)							{ m_title = title; }
	void					SetContent(string content)					{ m_content = content; }
	void					SetAuthorName(string name)					{ m_author_name = name; }
	void					SetAuthorID(FCULONG id)							{ m_authorID = id; }
	void					SetDateCreated(string date)					{ m_dateCreated = date; }
	void					SetMissionID(FCULONG id)						{ m_missionID = id; }
	void					SetLock(bool locked)								{ m_locked = locked; }

private:
	FCULONG				m_id;
	FCULONG				m_parentID;
	FCULONG				m_order;
	string				m_title,
								m_content,
								m_author_name;
	FCULONG				m_authorID;
	string				m_dateCreated;
	FCULONG				m_missionID;
	bool					m_locked;

	vector<ForumPost*> m_posts;
};

#endif//_FORUMPOST_H_