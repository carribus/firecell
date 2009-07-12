#include "../common/game_objects/swtypes.h"
#include "SWPortScan.h"
#include "SWLogicFactory.h"

SWLogicFactory* SWLogicFactory::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

SWLogicFactory::SWLogicFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

SWLogicFactory::~SWLogicFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

SWLogicFactory& SWLogicFactory::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new SWLogicFactory;
  }
  
  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void SWLogicFactory::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

ISoftwareLogic* SWLogicFactory::createLogic(FCSHORT logicType)
{
  ISoftwareLogic* pObj = NULL;

  switch ( logicType )
  {
  case  SWT_APP_PORTSCAN:
    pObj = new SWPortScan;
    break;

  default:
    break;
  }

  return pObj;
}