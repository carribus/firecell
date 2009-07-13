#include "ItemSoftware.h"

ItemSoftware::ItemSoftware(void)
: m_swType(0)
, m_bIsService(false)
, m_scriptID(0)
, m_cpuCost(0)
, m_memCost(0)
#ifndef _FCCLIENT_COMPILE_
, m_pLogic(NULL)
#endif
{
}

///////////////////////////////////////////////////////////////////////

ItemSoftware::~ItemSoftware(void)
{
#ifndef _FCCLIENT_COMPILE_
  if ( m_pLogic )
  {
    delete m_pLogic;
  }
#endif
}

///////////////////////////////////////////////////////////////////////

#ifndef _FCCLIENT_COMPILE_
bool ItemSoftware::Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result)
{
  bool bResult = false;

  if ( m_pLogic )
  {
    bResult = m_pLogic->Execute(pPlayer, cmd, args, result);
  }
  else
  {
    result = m_command + " cannot be used from the console.\n\n";
    bResult = true;
  }
  return bResult;
}
#endif