#ifdef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif
#include "../../common/game_objects/ItemType.h"
#include "FCApp.h"
#include "FCPlayerModel.h"
#include "ItemMgr.h"
#include "ItemHelper.h"

ItemSoftware* ItemHelper::getSoftwareItem(FCULONG itemID)
{
  ItemSoftware* pSoftware = NULL;
  FCPlayerModel* player = FCAPP->playerModel();
  ItemMgr& itemMgr = player->itemMgr();
  ItemMgr::GameItem gi;

  if ( itemMgr.getItem(itemID, gi) )
  {
    Item* pItem = gi.getItem();

    if ( pItem )
    {
      if ( pItem->GetTypeID() == ItemType::Software )
      {
        pSoftware = static_cast<ItemSoftware*>(pItem);
      }
    }
  }

  return pSoftware;
}