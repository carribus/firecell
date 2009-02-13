#include "ForumModel.h"

ForumModel* ForumModel::m_pThis = NULL;

ForumModel::ForumModel(void)
: m_pRootCat(NULL)
{
	m_pRootCat = new ForumCategory(0, 0, 0, 0, "", "");
}

///////////////////////////////////////////////////////////////////////

ForumModel::~ForumModel(void)
{
}

///////////////////////////////////////////////////////////////////////

ForumModel* ForumModel::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new ForumModel;
  }
  return m_pThis;
}

///////////////////////////////////////////////////////////////////////

void ForumModel::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

ForumCategory* ForumModel::addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, FCULONG threadCount, const std::string& name, const std::string& desc)
{
	ForumCategoryMap::iterator it = m_mapForumCategories.find(category_id);
	ForumCategory* pCat = NULL, *pParent = m_pRootCat;

	// check if we already have the category
	if ( it == m_mapForumCategories.end() )		
	{
		// we need to create the category
		if ( (pCat = new ForumCategory(category_id, parent_id, order, threadCount, name, desc)) )
		{
			m_mapForumCategories[category_id] = pCat;
			pParent = getCategoryByID(parent_id);
			if ( !pParent )
				pParent = m_pRootCat;

			pParent->addChild(pCat);
		}
	}
	else
	{
		// we have the category... 
		pCat = it->second;
    pCat->setName(name);
    pCat->setDesc(desc);
    pCat->setOrder(order);
    pCat->setThreadCount(threadCount);
	}

  return pCat;
}

///////////////////////////////////////////////////////////////////////

ForumCategory* ForumModel::getCategoryByID(FCULONG catID)
{
	ForumCategoryMap::iterator it = m_mapForumCategories.find(catID);

	if ( it == m_mapForumCategories.end() )
		return NULL;

	return it->second;
}
