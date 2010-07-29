#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include "../../common/game_objects/swtypes.h"
#include "AppLogicFactory.h"
#include "AppLogicForum.h"

AppLogicFactory::AppLogicFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

AppLogicFactory::~AppLogicFactory(void)
{
}

///////////////////////////////////////////////////////////////////////

IGameAppLogic* AppLogicFactory::createLogicForApp(ItemSoftware* pSoftware)
{
  IGameAppLogic* pLogic = NULL;

  if ( !pSoftware->IsService() )
  {
    switch ( pSoftware->GetSoftwareType() )
    {
    case  SWT_APP_FORUM:
      pLogic = new AppLogicForum(NULL);
      break;

    case  SWT_APP_NEWS:
      break;

    case  SWT_APP_EMAIL:
      break;

    case  SWT_APP_CONSOLE:
      break;

    case  SWT_APP_BANK:
      break;

    case  SWT_APP_CHAT:
      break;

    default:
      break;
    }
  }

  return pLogic;
}