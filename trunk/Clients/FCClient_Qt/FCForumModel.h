#ifndef FCFORUMMODEL_H
#define FCFORUMMODEL_H

#include <QAbstractItemModel>
#include <QReadWriteLock>
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
  Qt::ItemFlags flags(const QModelIndex& index) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

private:

	ForumCategory* getCategoryByID(FCULONG catID) const;

  ForumCategory*          m_rootItem;

  typedef std::map<FCULONG, ForumCategory*> ForumCategoryMap;
	ForumCategoryMap m_mapForumCategories;
  mutable QReadWriteLock m_lockForums;
};

#endif // FCFORUMMODEL_H
