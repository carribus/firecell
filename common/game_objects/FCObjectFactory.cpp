#include "FCObjectFactory.h"

FCObjectFactory* FCObjectFactory::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

FCObjectFactory::FCObjectFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

FCObjectFactory::~FCObjectFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

FCObjectFactory& FCObjectFactory::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new FCObjectFactory;
  }
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void FCObjectFactory::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}
