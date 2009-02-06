#ifndef _FORUMMODEL_H_
#define _FORUMMODEL_H_

#include "../../common/fctypes.h"
#include <string>
#include "ForumCategory.h"

class ForumModel
{
  ForumModel(void);
  ~ForumModel(void);

public:

  static ForumModel* instance();
  static void destroy();

  ForumCategory* addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, const std::string& name, const std::string desc);

private:

  static ForumModel* m_pThis;

};

#endif//_FORUMMODEL_H_