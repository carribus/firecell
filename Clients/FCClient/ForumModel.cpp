#include "ForumModel.h"

ForumModel* ForumModel::m_pThis = NULL;

ForumModel::ForumModel(void)
{
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

ForumCategory* ForumModel::addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, const std::string& name, const std::string desc)
{
  ForumCategory* pCat = NULL;

  return pCat;
}