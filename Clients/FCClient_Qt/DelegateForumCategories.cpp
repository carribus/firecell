#include <QPainter>
#include "ForumCategory.h"
#include "DelegateForumCategories.h"

DelegateForumCategories::DelegateForumCategories(QObject *parent)
: QAbstractItemDelegate(parent)
, m_fntHeader("Arial", 10, QFont::Bold)
, m_fntDesc("Arial", 10, QFont::Normal)
{

}

///////////////////////////////////////////////////////////////////////

DelegateForumCategories::~DelegateForumCategories()
{

}

///////////////////////////////////////////////////////////////////////

void DelegateForumCategories::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  ForumCategory* pCat = static_cast<ForumCategory*>(index.internalPointer());
  QRect r = option.rect;
  QLinearGradient gradient( r.center().x(), r.top(), r.center().x(), r.bottom() );

  painter->save();

  switch ( index.column() )
  {
  case  0:
    {
      if ( option.state & QStyle::State_Selected )
      {
        gradient.setColorAt(0, QColor( 255,255,255 ));
        gradient.setColorAt(1, QColor( 255,225,225 ));
        QBrush b(gradient);
        painter->fillRect(r, b);
      }
      else
      {
        gradient.setColorAt(0, QColor( 255,255,255 ));
        gradient.setColorAt(1, QColor( 225,225,255 ));
        QBrush b(gradient);
        painter->fillRect(r, b);
      }
      painter->setPen( QColor(0, 0, 0) );
      painter->drawRect( r );
      painter->setFont( m_fntHeader );
      r.setTopLeft( QPoint( r.left() + 5, r.top() + 2 ) );
      painter->drawText( r, Qt::TextSingleLine, pCat->getName() );
      painter->setFont( m_fntDesc );
      r.setTop( r.top() + 18 );
      r.setLeft( r.left() + 10 );
      painter->drawText( r, Qt::TextSingleLine, pCat->getDesc() );
    }
    break;

  case  1:
    {
      if ( option.state & QStyle::State_Selected )
      {
        gradient.setColorAt(0, QColor( 255,255,255 ));
        gradient.setColorAt(1, QColor( 255,225,225 ));
        QBrush b(gradient);
        painter->fillRect(r, b);
      }
      else
      {
        gradient.setColorAt(0, QColor( 255,255,255 ));
        gradient.setColorAt(1, QColor( 225,225,255 ));
        QBrush b(gradient);
        painter->fillRect(r, b);
      }
      painter->setPen( QColor(0, 0, 0) );
      painter->drawRect( r );
      painter->drawText( r, Qt::AlignCenter | Qt::AlignVCenter, QString("%1").arg( pCat->getThreadCount() ) );
    }
    break;

  }

  painter->restore();
}

///////////////////////////////////////////////////////////////////////

QSize DelegateForumCategories::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
  Q_UNUSED(option);
  Q_UNUSED(index);
  return QSize(400, 45);
}
