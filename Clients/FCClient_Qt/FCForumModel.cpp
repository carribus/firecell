#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include "FCForumModel.h"

#define CATEGORY_DATACOL_COUNT        3;

FCForumModel::FCForumModel(QObject *parent)
: QAbstractItemModel(parent)
, m_rootItem(NULL)
{
  m_rootItem = new ForumCategory(0, 0, 0, 0, "Root Node", "You should never see this node!", NULL);
}

///////////////////////////////////////////////////////////////////////

FCForumModel::~FCForumModel()
{
  if ( m_rootItem )
    delete m_rootItem;
}

///////////////////////////////////////////////////////////////////////

ForumCategory* FCForumModel::addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, FCULONG threadCount, const QString& name, const QString& desc)
{
	ForumCategoryMap::iterator it = m_mapForumCategories.find(category_id);
	ForumCategory* pCat = NULL, *pParent = m_rootItem;

	// check if we already have the category
	if ( it == m_mapForumCategories.end() )		
	{
		// we need to create the category
		if ( (pCat = new ForumCategory(category_id, parent_id, order, threadCount, name, desc, NULL)) )
		{
      QModelIndex pIdx;

			m_mapForumCategories[category_id] = pCat;
			pParent = getCategoryByID(parent_id);
			if ( !pParent )
				pParent = m_rootItem;

      pIdx = createParentModelIndex(pParent);

      beginInsertRows( pIdx, pParent->childCount(), pParent->childCount() );
			pParent->addChild(pCat);
      endInsertRows();
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

QModelIndex FCForumModel::index(int row, int column, const QModelIndex& parent) const
{
  if ( !hasIndex(row, column, parent) )
    return QModelIndex();

  ForumCategory* parentItem = NULL;

  if ( !parent.isValid() )
    parentItem = m_rootItem;
  else 
    parentItem = static_cast<ForumCategory*>(parent.internalPointer());

  ForumCategory* childItem = parentItem->childAt(row);
  if ( childItem )
    return createIndex(row, column, childItem);
  
  return QModelIndex();
}

///////////////////////////////////////////////////////////////////////

QModelIndex FCForumModel::parent(const QModelIndex& index) const
{
  if ( !index.isValid() )
    return QModelIndex();

  ForumCategory* childItem = static_cast<ForumCategory*>(index.internalPointer());
  ForumCategory* parentItem = childItem->getParent();

  if ( parentItem == m_rootItem )
    return QModelIndex();

  return createIndex(parentItem->currentIndex(), 0, parentItem);
}

///////////////////////////////////////////////////////////////////////

int FCForumModel::rowCount(const QModelIndex& parent) const
{
  if ( parent.column() > 0 )
    return 0;

  ForumCategory* parentItem = NULL;

  if ( !parent.isValid() )
    parentItem = m_rootItem;
  else
    parentItem = static_cast<ForumCategory*>(parent.internalPointer());

  return parentItem->childCount();
}

///////////////////////////////////////////////////////////////////////

int FCForumModel::columnCount(const QModelIndex& parent) const
{
  return CATEGORY_DATACOL_COUNT;
}

///////////////////////////////////////////////////////////////////////

QVariant FCForumModel::data(const QModelIndex& index, int role) const
{
  if ( !index.isValid() )
    return QVariant();

  if ( role != Qt::DisplayRole )
    return QVariant();

  ForumCategory* pItem = static_cast<ForumCategory*>(index.internalPointer());

  switch ( index.column() )
  {
  case  0:
    return pItem->getName();

  case  1:
    return pItem->getDesc();

  case  2:
    return (qulonglong)pItem->getThreadCount();

  default:
    break;
  }

  return QVariant();
}

///////////////////////////////////////////////////////////////////////
/*
 * insertRows() and removeRows() are not needed for this model since it will not be editable
 * from the UI
 *

bool FCForumModel::insertRows(int position, int count, const QModelIndex& parent)
{
  if ( !parent.isValid() )
    return false;

  ForumCategory* parentItem = NULL;

  if ( !parent.isValid() )
    parentItem = m_rootItem;
  else
    parentItem = static_cast<ForumCategory*>(parent.internalPointer());
  
  beginInsertRows(parent, position, position + count - 1);
  for ( int i = 0; i < count; i++ )
  {
    parentItem->addChild( new ForumCategory(0, parentItem->getID(), 0, 0, "", "", parentItem) );
  }

  endRemoveRows();

  return true;
}

///////////////////////////////////////////////////////////////////////

bool FCForumModel::removeRows(int row, int count, const QModelIndex& parent)
{
  return false;
}
*/

///////////////////////////////////////////////////////////////////////

ForumCategory* FCForumModel::getCategoryByID(FCULONG catID)
{
	ForumCategoryMap::iterator it = m_mapForumCategories.find(catID);

	if ( it == m_mapForumCategories.end() )
		return NULL;

	return it->second;
}

///////////////////////////////////////////////////////////////////////

QModelIndex FCForumModel::createParentModelIndex(ForumCategory* pCat)
{
  if ( !pCat )
    return QModelIndex();

  ForumCategory* pParent = pCat->getParent();

  return index( pCat->currentIndex(), 0, pParent ? createParentModelIndex(pCat->getParent()) : index(0, 0, QModelIndex()) );
}