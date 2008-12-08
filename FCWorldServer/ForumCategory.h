#ifndef _FORUMCATEGORY_H_
#define _FORUMCATEGORY_H_

#include <string>
#include "../common/fctypes.h"

using namespace std;

class ForumCategory
{
public:
	ForumCategory()
		: m_id(0), m_parentID(0), m_accountTypeRequired(0), m_minLevel(0), m_maxLevel(0)
	{
	}

	~ForumCategory()
	{
	}

	FCULONG						GetID()												{ return m_id; }
	FCULONG						GetParentID()									{ return m_parentID; }
	string						GetName()											{ return m_name; }
	string						GetDescription()							{ return m_description; }
	FCULONG						GetAccountTypeRequired()			{ return m_accountTypeRequired; }
	FCULONG						GetMinLevel()									{ return m_minLevel; }
	FCULONG						GetMaxLevel()									{ return m_maxLevel; }

	void							SetID(FCULONG id)							{ m_id = id; }
	void							SetParentID(FCULONG id)				{ m_parentID = id; }
	void							SetName(string name)					{ m_name = name; }
	void							SetDescription(string desc)		{ m_description = desc; }
	void							SetAccountTypeRequired(FCULONG type)		{ m_accountTypeRequired = type; }
	void							SetMinLevel(FCULONG lvl)			{ m_minLevel = lvl; }
	void							SetMaxLevel(FCULONG lvl)			{ m_maxLevel = lvl; }

private:

	FCULONG						m_id;
	FCULONG						m_parentID;
	string						m_name;
	string						m_description;
	FCULONG						m_accountTypeRequired;
	FCULONG						m_minLevel;
	FCULONG						m_maxLevel;
};

#endif//_FORUMCATEGORY_H_