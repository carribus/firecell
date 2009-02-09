#ifndef _FORUMMODEL_H_
#define _FORUMMODEL_H_

#include <map>
#include <string>
#include "../../common/fctypes.h"
#include "ForumCategory.h"

/**
 *	@class ForumModel
 *	@brief Data model for the Forum portion of FireCell
 */
class ForumModel
{
  ForumModel(void);
  ~ForumModel(void);

public:

	/**
	 *	@brief Get access to the singleton instance of the ForumModel
	 */
  static ForumModel* instance();

	/**
	 *	@brief Destroy the singleton instance of the ForumModel
	 */
  static void destroy();

	/**
	 *	@brief Adds/updates a category in the ForumModel
	 *	
	 *	If the category already exists (based on category_id), the existing ForumCategory object will be fetched and 
	 *	then returned as the result. If the ForumCategory is created, it will be populated with the data and returned
	 */
  ForumCategory* addCategory(FCULONG category_id, FCULONG parent_id, FCULONG order, FCULONG threadCount, const std::string& name, const std::string& desc);

	ForumCategory* getCategoryByID(FCULONG catID);
	ForumCategory* getRootCategory()										{ return m_pRootCat; }

private:

  static ForumModel*	m_pThis;
	ForumCategory*			m_pRootCat;

	typedef std::map<FCULONG, ForumCategory*> ForumCategoryMap;
	ForumCategoryMap m_mapForumCategories;

};

#endif//_FORUMMODEL_H_