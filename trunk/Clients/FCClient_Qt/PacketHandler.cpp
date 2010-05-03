#ifndef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include "PacketHandler.h"
#include "../../common/protocol/fcprotocol.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "../../common/game_objects/ItemType.h"
#include "FCApp.h"
#include "FCPlayerModel.h"

PacketHandler::PacketHandler(FCApp* pApp)
: m_pApp(pApp)
{
}

///////////////////////////////////////////////////////////////////////

PacketHandler::~PacketHandler(void)
{
}

///////////////////////////////////////////////////////////////////////

void PacketHandler::onCommand(PEPacket* pPkt)
{
  if ( !pPkt )
    return;
}

///////////////////////////////////////////////////////////////////////

void PacketHandler::onResponse(PEPacket* pPkt)
{
  if ( !pPkt )
    return;

  bool bHandled = false;
  FCSHORT msgID;

  pPkt->GetField("msg", &msgID, sizeof(FCSHORT));
  switch ( msgID )
  {
  case  FCMSG_INFO_SERVER:
    {
      bHandled = onResponseServerInfo(pPkt);
    }
    break;

  case  FCMSG_LOGIN:
    {
      bHandled = onResponseLogin(pPkt);
    }
    break;

  case  FCMSG_GETCHARACTERS:
    {
      bHandled = onResponseGetCharacters(pPkt);
    }
    break;

  case  FCMSG_SELECT_CHARACTER:
    {
      bHandled = onResponseSelectCharacter(pPkt);
    }
    break;

  case  FCMSG_CHARACTER_ITEMS_REQUEST:
    {
      bHandled = onResponseCharacterItemsRequest(pPkt);
    }
    break;

  case  FCMSG_CHARACTER_ASSET_REQUEST:
    {
      bHandled = onResponseCharacterAssetsRequest(pPkt);
    }
    break;

  default:
    qDebug() << "PacketHandler::onResponse -- Unknown Response message (" << msgID << ")";
    break;
  }
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseServerInfo(PEPacket* pPkt)
{
  if ( m_pApp->getState().state <= AppStateLogin )
  {
    __FCPKT_INFO_SERVER d;

    getPacketData<__FCPKT_INFO_SERVER>(pPkt, d);

    emit serverInfoReceived(d.verMajor, d.verMinor);

    m_pApp->setState( AppStateLogin );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseLogin(PEPacket* pPkt)
{
  __FCPKT_LOGIN_RESP d;

  getPacketData<__FCPKT_LOGIN_RESP>(pPkt, d);
  if ( d.loginStatus == LoginSuccess )
  {
    m_pApp->setStateStep( AppState_Login_LoginSucceeded );
    m_pApp->network().requestCharacterInfo();
  }
  else
  {
    m_pApp->setStateStep( AppState_Login_LoginFailed );
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseGetCharacters(PEPacket* pPkt)
{
  __FCPKT_CHARACTER_LIST d;

  getPacketData<__FCPKT_CHARACTER_LIST>(pPkt, d);

  for ( FCBYTE i = 0; i < d.numCharacters; i++ )
  {
    Character* c = new Character(d.characters[i].character_id, d.characters[i].name);

    c->SetCityID(d.characters[i].city_id);
    c->SetCountryID(d.characters[i].country_id);
    c->SetFameScale(d.characters[i].fame_scale);
    c->SetLevel(d.characters[i].level);
    c->SetXP(d.characters[i].xp);

    m_pApp->playerModel()->addCharacter(c);
  }

  m_pApp->setState(AppStateCharacterSelection);

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseSelectCharacter(PEPacket* pPkt)
{
  __FCPKT_SELECT_CHARACTER_RESP d;

  getPacketData<__FCPKT_SELECT_CHARACTER_RESP>(pPkt, d);

  switch ( d.status )
  {
  case  CharacterSelectSucceeded:
    {
      m_pApp->setStateStep(AppState_CharacterSelection_CharacterSelected);
      if ( m_pApp->playerModel()->selectCharacter(d.character_id) )
        m_pApp->network().requestCharacterItems(d.character_id);
    }
    break;

  case  CharacterSelectFailed:
    m_pApp->setStateStep(AppState_CharacterSelection_Failed);
    break;

  case  CharacterSelectFailedInWorld:
    break;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseCharacterItemsRequest(PEPacket* pPkt)
{
	__FCPKT_CHARACTER_ITEMS_REQUEST_RESP* d;
	size_t dataLen;
	Item* pItem = NULL;
	ItemSoftware* pSoftware = NULL;
  ItemMgr& itemMgr = m_pApp->playerModel()->itemMgr();

	m_pApp->setState( AppStatePlaying );

	pPkt->GetField("dataLen", &dataLen, sizeof(size_t));
	d = (__FCPKT_CHARACTER_ITEMS_REQUEST_RESP*) new FCBYTE[ dataLen ];
	pPkt->GetField("data", d, dataLen);

	for (FCULONG i = 0; i < d->itemCount; i++ )
	{
		pItem = itemMgr.addItem( d->software[i].item_id,
						  							 d->software[i].name,
														 d->software[i].itemtype_id,
														 d->software[i].min_level,
														 d->software[i].max_level,
														 d->software[i].npc_value );
		if ( pItem && pItem->GetTypeID() == ItemType::Software )
		{
			pSoftware = (ItemSoftware*)pItem;
			pSoftware->SetSoftwareType( d->software[i].softwareTypeID );
			pSoftware->IsService( d->software[i].is_service );
      pSoftware->SetCPUCost( d->software[i].cpu_cost );
      pSoftware->SetMemCost( d->software[i].mem_cost );
			itemMgr.setItemCount( pItem->GetID(), d->software[i].itemCount );

      //
      // NOTE: Not sure what this was supposed to be doing in here from the old code... but I think it was some sort of hack.
      //       Will check it out later and remove if its useless.
      //

      // if the software is not a service, then notify the desktop that a new program is available.
      if ( !pSoftware->IsService() && d->software[i].desktop_icon_flag )
      {
//        FireEvent( FCME_ApplicationAdded, pSoftware );
      }
		}
	}

  m_pApp->network().requestCharacterAssets( m_pApp->playerModel()->getCurrentCharacter()->GetID() );

  delete [] (FCBYTE*)d;

	return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseCharacterAssetsRequest(PEPacket* pPkt)
{
  return true;
}

///////////////////////////////////////////////////////////////////////

void PacketHandler::onError(PEPacket* pPkt)
{
  if ( !pPkt )
    return;
}
