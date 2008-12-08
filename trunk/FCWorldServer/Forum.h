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

	size_t AddCategory(FCULONG id, FCULONG parentID, string name, string desc, FCULONG accountTypeReq, FCULONG minLevel, FCULONG maxLevel);
	size_t GetCategoryCount();
	size_t GetCategoriesForPlayer(const Player* pPlayer, vector<ForumCategory*>& target);
	ForumCategory* GetCategoryByID(FCULONG id);

private:

	vector<ForumCategory*>			m_categories;
};

#endif//_FORUM_H_