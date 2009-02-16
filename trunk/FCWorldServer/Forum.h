#ifndef _FORUM_H_
#define _FORUM_H_

#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "../common/PThreadMutex.h"
#include "IEventSystem.h"
#include "ForumCategory.h"
#include "Player.h"

using namespace std;

class Forum : public IEventSource
{
public:

  DECLARE_EVENT_SOURCE();
  DECLARE_EVENT(NewCategory);
  DECLARE_EVENT(NewThread);
  DECLARE_EVENT(NewReply);

	Forum(void);
	~Forum(void);

	size_t AddCategory(FCULONG id, FCULONG parentID, FCULONG order, string name, string desc, FCULONG accountTypeReq, FCULONG minLevel, FCULONG maxLevel);
	size_t GetCategoryCount();
	size_t GetCategoriesForPlayer(const Player* pPlayer, vector<ForumCategory*>& target);
	ForumCategory* GetCategoryByID(FCULONG id);

  bool CreateNewForumThread(FCULONG category_id, FCULONG author_id, std::string title, std::string content);
	ForumPost* AddForumPost(FCULONG id, FCULONG parentID, FCULONG category_id, FCULONG order, string title, string content, FCULONG author_id, string date_created, FCULONG mission_id = 0);
	size_t GetForumThreads(FCULONG category_id, vector<ForumPost*>& target);

  /*
   *  IEventSource implementation
   */
  const string& GetType()                 { return Forum::EVTSYS_ObjectType; }

private:

  FCULONG GetNewPostID();

  PThreadMutex                m_mutexCategories;
	vector<ForumCategory*>			m_categories;
  PThreadMutex                m_mutexPostID;
  FCULONG                     m_lastPostID;
};

#endif//_FORUM_H_