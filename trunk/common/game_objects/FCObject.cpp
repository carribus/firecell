#include "FCObject.h"

FCObject::FCObject(const std::string& typeName, FCObject* parent)
: m_parent(parent)
, m_typeName(typeName)
{
  if ( m_parent )
    m_parent->addChild(this);
}

FCObject::~FCObject(void)
{
  deleteChildren();
}

FCObject* FCObject::getParent()
{
  return m_parent;
}

size_t FCObject::getChildCount()
{
  return m_children.size();
}

FCObject* FCObject::getChild(size_t index)
{
  if ( index >= m_children.size() )
    return NULL;

  ReadLock();
  FCObject* pObj = m_children[index];
  Unlock();

  return pObj;
}

bool FCObject::addChild(FCObject* pChild)
{
  if ( !pChild )
    return false;

  WriteLock();
  m_children.push_back(pChild);
  Unlock();

  return true;
}

void FCObject::remove()
{
  if ( m_parent )
  {
    m_parent->removeChild(this);
  }
}

void FCObject::removeChild(FCObject* pChild)
{
  WriteLock();
  std::vector<FCObject*>::iterator it = m_children.begin();
  std::vector<FCObject*>::iterator limit = m_children.end();

  for ( ; it != limit; it++ )
  {
    if ( (*it) == pChild )
    {
      m_children.erase(it);
      break;
    }
  }
  Unlock();
}

void FCObject::deleteChildren()
{
  WriteLock();
  std::vector<FCObject*>::iterator it = m_children.begin();
  std::vector<FCObject*>::iterator limit = m_children.end();

  while ( m_children.size() )
  {
    delete *it;
    m_children.erase(it);
  }
  Unlock();
}

std::string FCObject::getTypeName()
{
  return m_typeName;
}

void FCObject::ReadLock()
{
}

void FCObject::WriteLock()
{
}

void FCObject::Unlock()
{
}
