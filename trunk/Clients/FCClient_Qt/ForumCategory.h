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

	FCULONG getID()																	{ return m_id; }
	FCULONG getParentID()														{ return m_parentID; }
	FCULONG getOrder()															{ return m_order; }
  FCULONG getThreadCount()                        { return m_threadCount; }
	QString getName()														    { return m_name; }
	QString getDesc()														    { return m_desc; }
	ForumCategory* getParent()											{ return m_parent; }

  void setOrder(FCULONG order)                    { m_order = order; }
  void setThreadCount(FCULONG count)              { m_threadCount = count; }
  void setName(const QString& name)               { m_name = name; }
  void setDesc(const QString& desc)               { m_desc = desc; }
	void setParent(ForumCategory* pParent)					{ m_parent = pParent; }

private:

  FCULONG                       m_id;
  FCULONG                       m_parentID;
  FCULONG                       m_order;
  FCULONG                       m_threadCount;
  QString                       m_name;
  QString                       m_desc;
  QList<ForumCategory*>         m_children;
  ForumCategory*                m_parent;
};

#endif//_FORUMCATEGORY_H_