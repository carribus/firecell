#ifndef _SWLOGICFACTORY_H_
#define _SWLOGICFACTORY_H_

#include "../common/fctypes.h"
#include "../common/interfaces/ISoftwareLogic.h"

class SWLogicFactory
{
  SWLogicFactory(void);
  ~SWLogicFactory(void);

public:

  static SWLogicFactory& instance();
  static void destroy();

  ISoftwareLogic* createLogic(FCSHORT logicType);

private:

  static SWLogicFactory* m_pThis;

};

#endif//_SWLOGICFACTORY_H_