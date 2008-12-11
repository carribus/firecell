#ifndef _FORUM_H_
#define _FORUM_H_

#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "ForumCategory.h"
#include "Player.h"

using namespace std;

class Forum
{
public:
	Forum(void);
	~Forum(void);

	size_t AddCategory(FCULONG id, FCULONG parentID, FCULONG order, string name, string desc, FCULONG accountTypeReq, FCULONG minLevel, FCULONG maxLevel);
	size_t GetCategoryCount();
	size_t GetCategoriesForPlayer(const Player* pPlayer, vector<ForumCategory*>& target);
	ForumCategory* GetCategoryByID(FCULONG id);

	size_t AddForumPost(FCULONG id, FCULONG parentID, FCULONG category_id, FCULONG order, string title, string content, FCULONG author_id, string date_created, FCULONG mission_id = 0);
	size_t GetForumThreads(FCULONG category_id, vector<ForumPost>& target);

private:

	vector<ForumCategory*>			m_categories;
};

#endif//_FORUM_H_