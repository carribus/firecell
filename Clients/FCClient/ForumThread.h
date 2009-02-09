#ifndef _FORUMTHREAD_H_
#define _FORUMTHREAD_H_

#include "../../common/fctypes.h"
#include <string>

class ForumThread
{
public:
	ForumThread(void);
	ForumThread(FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID);
	~ForumThread(void);

private:

	FCULONG m_thread_id;
	FCULONG m_parent_id;
	FCULONG m_order;
	std::string m_title;
	FCULONG m_author_id;
	std::string m_author_name;
	std::string m_date_created;
	FCULONG m_mission_id;
};

#endif//_FORUMTHREAD_H_