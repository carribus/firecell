#include "../common/game_objects/FCObjectFactory.h"
#include "Company.h"

Company::Company(FCObject* parent)
: FCObject("Company", parent)
, m_id(0)
, m_computer(NULL)
{
  m_computer = FCObjectFactory::instance().createObject<Computer>(this);
}

Company::~Company(void)
{
}
