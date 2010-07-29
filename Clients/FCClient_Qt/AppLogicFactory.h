#ifndef _APPLOGICFACTORY_H_
#define _APPLOGICFACTORY_H_

#include "../../common/fctypes.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "IGameAppLogic.h"

class AppLogicFactory
{
public:
  AppLogicFactory(void);
  ~AppLogicFactory(void);

  IGameAppLogic* createLogicForApp(ItemSoftware* pSoftware);
};

#endif//_APPLOGICFACTORY_H_