#include "PlayerBase.h"

PlayerBase::PlayerBase(const std::string typeName)
: FCObject(typeName.size() > 0 ? typeName : "PlayerBase")
, m_accountID(0)
, m_id(0)
, m_xp(0)
, m_level(0)
, m_fameScale(0)
, m_cityID(0)
, m_countryID(0)
, m_clientSocket(0)
, m_pRouterSocket(NULL)
{
}

///////////////////////////////////////////////////////////////////////

PlayerBase::PlayerBase(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, const std::string typeName)
: FCObject(typeName.size() > 0 ? typeName : "PlayerBase")
, m_accountID(accountID)
, m_id(id)
, m_name(name)
, m_email(email)
, m_xp(xp)
, m_level(level)
, m_fameScale(fameScale)
, m_cityID(cityID)
, m_countryID(countryID)
, m_clientSocket(0)
, m_pRouterSocket(NULL)
{
}

///////////////////////////////////////////////////////////////////////

PlayerBase::~PlayerBase(void)
{
}
