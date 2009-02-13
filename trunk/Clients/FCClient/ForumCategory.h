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
  ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, FCULONG thread_count, const std::string& name, const std::string& desc);
  ~ForumCategory(void);

	bool addChild(ForumCategory* pCategory);
	const ForumCategoryMap& getChildren()						{ return m_mapCategories; }

	bool addThread(FCULONG thread_id, FCULONG parent_id, FCULONG order, std::string title, FCULONG author_id,	std::string author_name, std::string date_created, FCULONG mission_id);
	ForumThread* getThread(FCULONG thread_id);
  const ForumThreadMap& getThreads()              { return m_mapThreads; }      

	FCULONG getID()																	{ return m_id; }
	FCULONG getParentID()														{ return m_parentID; }
	FCULONG getOrder()															{ return m_order; }
  FCULONG getThreadCount()                        { return m_threadCount; }
	std::string getName()														{ return m_name; }
	std::string getDesc()														{ return m_desc; }
	ForumCategory* getParent()											{ return m_pParent; }

  void setOrder(FCULONG order)                    { m_order = order; }
  void setThreadCount(FCULONG count)              { m_threadCount = count; }
  void setName(const std::string& name)           { m_name = name; }
  void setDesc(const std::string& desc)           { m_desc = desc; }
	void setParent(ForumCategory* pParent)					{ m_pParent = pParent; }

private:

  FCULONG m_id;
  FCULONG m_parentID;
  FCULONG m_order;
  FCULONG m_threadCount;
  std::string m_name;
  std::string m_desc;
	ForumCategory* m_pParent;

	ForumCategoryMap m_mapCategories;
	ForumThreadMap	m_mapThreads;
};

#endif//_FORUMCATEGORY_H_