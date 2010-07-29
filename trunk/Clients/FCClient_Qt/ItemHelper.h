#ifndef _ITEMHELPER_H_
#define _ITEMHELPER_H_

#include "../../common/fctypes.h"
#include "../../common/game_objects/ItemSoftware.h"

class ItemHelper
{
  ItemHelper(void)  {}
  ~ItemHelper(void) {}

public:

  static ItemSoftware* getSoftwareItem(FCULONG itemID);
};

#endif//_ITEMHELPER_H_
