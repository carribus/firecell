#include "Character.h"

Character::Character(FCUINT id, const QString& name)
: m_id(id)
, m_name(name)
, m_xp(0)
, m_level(0)
, m_fame_scale(0)
, m_country_id(0)
, m_city_id(0)
, m_computer(NULL)
{
  m_computer = new ComputerBase(NULL);
}

///////////////////////////////////////////////////////////////////////

Character::Character(const Character& src)
: m_computer(NULL)
{
	*this = src;
	m_computer = new ComputerBase(NULL);
}

///////////////////////////////////////////////////////////////////////

Character::~Character(void)
{
  if ( m_computer )
    delete m_computer;
}

///////////////////////////////////////////////////////////////////////
