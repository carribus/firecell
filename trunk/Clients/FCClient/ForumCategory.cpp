#include "ForumCategory.h"

ForumCategory::ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, const std::string& name, const std::string& desc)
: m_id(id)
, m_parentID(parent_id)
, m_order(order)
, m_name(name)
, m_desc(desc)
{
}

///////////////////////////////////////////////////////////////////////

ForumCategory::~ForumCategory(void)
{
}

///////////////////////////////////////////////////////////////////////

bool ForumCategory::addChild(ForumCategory* pCategory)
{
	if ( !pCategory )
		return false;

	ForumCategoryMap::iterator it = m_mapCategories.find( pCategory->getID() );

	if ( it == m_mapCategories.end() )
	{
		m_mapCategories[ pCategory->getID() ] = pCategory;
		return true;
	}

	return false;
}

///////////////////////////////////////////////////////////////////////

bool ForumCategory::addThread(FCULONG thread_id, FCULONG parent_id, FCULONG order, std::string title, FCULONG author_id, std::string author_name, std::string date_created, FCULONG mission_id)
{
	ForumThread* pThread = getThread(thread_id);

	if ( !pThread )
	{
		pThread = new ForumThread(thread_id, 
															parent_id,
															order,
															title,
															author_id,
															author_name,
															date_created,
															mission_id);
		m_mapThreads[thread_id] = pThread;
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

ForumThread* ForumCategory::getThread(FCULONG thread_id)
{
	ForumThreadMap::iterator it = m_mapThreads.find(thread_id);

	if ( it != m_mapThreads.end() )
		return (it->second);

	return NULL;
}