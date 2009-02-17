#ifndef _FORUMTHREAD_H_
#define _FORUMTHREAD_H_

#include "../../common/fctypes.h"
#include <string>
#include <vector>

class ForumThread
{
public:
  typedef std::vector<ForumThread*> ThreadVector;

  ForumThread(void);
	ForumThread(FCULONG categoryID, FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID);
	~ForumThread(void);

  ForumThread* addPost(FCULONG threadID, FCULONG parentID, FCULONG order, std::string title, FCULONG authorID, std::string authorName, std::string dateCreated, FCULONG missionID);
  ForumThread* getPost(FCULONG threadID);
  void clearAllPosts();
  ThreadVector& getAllPosts()                     { return m_posts; }

  FCULONG getCategoryID()                         { return m_category_id; }
  FCULONG getThreadID()                           { return m_thread_id; }
  FCULONG getParentID()                           { return m_parent_id; }
  FCULONG getOrder()                              { return m_order; }
  std::string getTitle()                          { return m_title; }
  FCULONG getAuthorID()                           { return m_author_id; }
  std::string getAuthorName()                     { return m_author_name; }
  std::string getDateCreated()                    { return m_date_created; }
  std::string getContent()                        { return m_content; }
  FCULONG getMissionID()                          { return m_mission_id; }

	void setTitle(const std::string& title)					{ m_title = title; }
  void setContentParams(FCULONG contentIndex, FCULONG contentLen)       { m_contentIndex = contentIndex; m_contentLen = contentLen; }
  void setContent(const std::string& content)     { m_content = content; }
  FCULONG getContentIndex()                       { return m_contentIndex; }
  FCULONG getContentLen()                         { return m_contentLen; }

private:

  FCULONG m_category_id;
	FCULONG m_thread_id;
	FCULONG m_parent_id;
	FCULONG m_order;
	std::string m_title;
	FCULONG m_author_id;
	std::string m_author_name;
	std::string m_date_created;
  std::string m_content;
	FCULONG m_mission_id;

  FCULONG m_contentIndex,
          m_contentLen;

  ThreadVector m_posts;
};

#endif//_FORUMTHREAD_H_