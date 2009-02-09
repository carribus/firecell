#ifndef _FORUMCATEGORY_H_
#define _FORUMCATEGORY_H_

#include <map>
#include <string>
#include "../../common/fctypes.h"
#include "ForumThread.h"

class ForumCategory;
typedef std::map<FCULONG, ForumCategory*> ForumCategoryMap;
typedef std::map<FCULONG, ForumThread*> ForumThreadMap;

class ForumCategory
{
public:
  ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, const std::string& name, const std::string& desc);
  ~ForumCategory(void);

	bool addChild(ForumCategory* pCategory);
	const ForumCategoryMap& getChildren()						{ return m_mapCategories; }

	bool addThread(FCULONG thread_id, FCULONG parent_id, FCULONG order, std::string title, FCULONG author_id,	std::string author_name, std::string date_created, FCULONG mission_id);
	ForumThread* getThread(FCULONG thread_id);

	FCULONG getID()																	{ return m_id; }
	FCULONG getParentID()														{ return m_parentID; }
	FCULONG getOrder()															{ return m_order; }
	std::string getName()														{ return m_name; }
	std::string getDesc()														{ return m_desc; }

private:

  FCULONG m_id;
  FCULONG m_parentID;
  FCULONG m_order;
  std::string m_name;
  std::string m_desc;

	ForumCategoryMap m_mapCategories;
	ForumThreadMap	m_mapThreads;
};

#endif//_FORUMCATEGORY_H_