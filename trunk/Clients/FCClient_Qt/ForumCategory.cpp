#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include "ForumCategory.h"

ForumCategory::ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, FCULONG thread_count, const QString& name, const QString& desc, ForumCategory* parent)
: m_parent(parent)
, m_id(id)
, m_parentID(parent_id)
, m_order(order)
, m_threadCount(thread_count)
, m_name(name)
, m_desc(desc)
{
}

///////////////////////////////////////////////////////////////////////

ForumCategory::~ForumCategory(void)
{
  qDeleteAll(m_children);
}

///////////////////////////////////////////////////////////////////////

bool ForumCategory::addChild(ForumCategory* pCategory)
{
	if ( !pCategory )
		return false;

  m_children.push_back(pCategory);
  pCategory->setParent(this);
  return true;
}

///////////////////////////////////////////////////////////////////////

ForumCategory* ForumCategory::childAt(int nIndex)
{
  if ( nIndex < 0 || nIndex >= m_children.count() )
    return NULL;

  return m_children.at(nIndex);
}

///////////////////////////////////////////////////////////////////////

int ForumCategory::currentIndex()
{
  if ( m_parent )
    return m_parent->m_children.indexOf(const_cast<ForumCategory*>(this));

  return 0;
}