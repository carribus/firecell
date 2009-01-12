#include "Character.h"

Character::Character(FCUINT id, const wstring& name)
: m_id(id)
, m_name(name)
, m_xp(0)
, m_level(0)
, m_fame_scale(0)
, m_country_id(0)
, m_city_id(0)
{
}

Character::~Character(void)
{
}
