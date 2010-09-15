#ifndef _FORUMCATEGORY_H_
#define _FORUMCATEGORY_H_

#include "../../common/fctypes.h"
#include <QList>
#include <QString>

class ForumCategory
{
public:
  ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, FCULONG thread_count, const QString& name, const QString& desc, ForumCategory* parent);
  ~ForumCategory(void);

	bool addChild(ForumCategory* pCategory);
  ForumCategory* childAt(int nIndex);
  int currentIndex();
  int childCount()                                { return m_children.count(); }

	FCULONG getID()	const 													{ return m_id; }
	FCULONG getParentID()	const											{ return m_parentID; }
	FCULONG getOrder() const												{ return m_order; }
  FCULONG getThreadCount() const                  { return m_threadCount; }
	QString getName() const											    { return m_name; }
	QString getDesc()	const 										    { return m_desc; }
	ForumCategory* getParent()											{ return m_parent; }

  void setOrder(FCULONG order)                    { m_order = order; }
  void setThreadCount(FCULONG count)              { m_threadCount = count; }
  void setName(const QString& name)               { m_name = name; }
  void setDesc(const QString& desc)               { m_desc = desc; }
	void setParent(ForumCategory* pParent)					{ m_parent = pParent; }

private:

  ForumCategory*                m_parent;
  FCULONG                       m_id;
  FCULONG                       m_parentID;
  FCULONG                       m_order;
  FCULONG                       m_threadCount;
  QString                       m_name;
  QString                       m_desc;
  QList<ForumCategory*>         m_children;
};

#endif//_FORUMCATEGORY_H_
