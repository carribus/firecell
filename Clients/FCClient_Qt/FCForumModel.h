#ifndef FCFORUMMODEL_H
#define FCFORUMMODEL_H

#include <QAbstractItemModel>
#include <map>
#include "ForumCategory.h"

class FCForumModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  FCForumModel(QObject *parent);
  ~FCForumModel();

  /**
   *  @brief Convenience function to add categories to the model
   */
  ForumCategory* addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, FCULONG threadCount, const QString& name, const QString& desc);

  //
  // QAbstractItemModel implementation
  QModelIndex index(int row, int column, const QModelIndex& parent) const;
  QModelIndex parent(const QModelIndex& index) const;
  int rowCount(const QModelIndex& parent = QModelIndex()) const;
  int columnCount(const QModelIndex& parent = QModelIndex()) const;
  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
/*
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex());
  bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());
*/
private:

	ForumCategory* getCategoryByID(FCULONG catID);

	/**
	 *	@brief Recursive QModelIndex generator for parents
	 *	
	 *	This function is used to generate a valid QModelIndex for any item within the tree hierarchy.
   *  QModelIndex objects need to have the correct parent set at all times. This function recursively creates
   *  valid QModelIndex objects for the parent of prior QModelIndex objects.
   *
   *  Eg, if a model function requires you to pass a QModelIndex object for a parent, use the function as follows:
   *
   *    ForumCategory* pCategoryOfInterest;
   *    QModelIndex validIndexForCategory = index( pCategoryOfInterest->currentIndex(), 0, createParentModelIndex(pCategoryOfInterest) );
	 */
  QModelIndex createParentModelIndex(ForumCategory* pCat);

  ForumCategory*          m_rootItem;

  typedef std::map<FCULONG, ForumCategory*> ForumCategoryMap;
	ForumCategoryMap m_mapForumCategories;

};

#endif // FCFORUMMODEL_H
