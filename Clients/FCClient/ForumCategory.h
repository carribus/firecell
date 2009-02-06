#ifndef _FORUMCATEGORY_H_
#define _FORUMCATEGORY_H_

#include "../../common/fctypes.h"
#include <string>

class ForumCategory
{
public:
  ForumCategory(FCULONG id, FCULONG parent_id, FCULONG order, const std::string& name, const std::string& desc);
  ~ForumCategory(void);

private:

  FCULONG m_id;
  FCULONG m_parentID;
  FCULONG m_order;
  std::string name;
  std::string desc;
};

#endif//_FORUMCATEGORY_H_