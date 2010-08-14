#ifndef DELEGATEFORUMCATEGORIES_H
#define DELEGATEFORUMCATEGORIES_H

#include <QAbstractItemDelegate>
#include <QFont>
#include <QLinearGradient>

class DelegateForumCategories : public QAbstractItemDelegate
{
  Q_OBJECT

public:
    DelegateForumCategories(QObject *parent);
    ~DelegateForumCategories();

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    
  QFont             m_fntHeader;
  QFont             m_fntDesc;
};

#endif // DELEGATEFORUMCATEGORIES_H
