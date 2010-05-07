/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

                Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _USE_STDAFX_H_
  #include "StdAfx.h"
#endif//_USE_STDAFX_H_
#include "PacketHandler.h"
#include "../../common/protocol/fcprotocol.h"
#include "../../common/game_objects/ItemSoftware.h"
#include "../../common/game_objects/ItemType.h"
#include "FCApp.h"
#include "FCPlayerModel.h"
#include "ItemMgr.h"
#include "FCMissionMgr.h"

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

  case  FCMSG_CHARACTER_MISSIONS_REQUEST:
    {
      bHandled = onResponseCharacterMissionsRequest(pPkt);
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
	__FCPKT_CHARACTER_ITEMS_REQUEST_RESP* d = NULL;
	Item* pItem = NULL;
	ItemSoftware* pSoftware = NULL;
  ItemMgr& itemMgr = m_pApp->playerModel()->itemMgr();

	m_pApp->setState( AppStatePlaying );

  getDynamicPacketData<__FCPKT_CHARACTER_ITEMS_REQUEST_RESP>(pPkt, d);

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
  if ( !FCAPP->playerModel()->getCurrentCharacter() )
    return false;

  __FCPKT_CHARACTER_ASSET_REQUEST_RESP d;
  Character* pCharacter = FCAPP->playerModel()->getCurrentCharacter();
  InGameIPAddress& ip = pCharacter->GetIP();
  ComputerBase& comp = pCharacter->GetComputer();
  ItemMgr& itemMgr = FCAPP->playerModel()->itemMgr();

  getPacketData<__FCPKT_CHARACTER_ASSET_REQUEST_RESP>(pPkt, d);

  // set the player's IP address
  ip.SetIP( d.ip_address.a, d.ip_address.b, d.ip_address.c, d.ip_address.d );
  
  // update the player's computer details
  comp.SetID(d.computer.id);
  comp.SetName(d.computer.name);
  comp.SetHDDSize(d.computer.hddSize);
  comp.SetNetworkSpeed(d.computer.networkSpeed);

  // update the player's processor details
  comp.GetProcessor().SetItemInfo( d.computer.processor.item_id, d.computer.processor.name, d.computer.processor.itemtype_id,
                                   d.computer.processor.min_level, d.computer.processor.max_level, d.computer.processor.npc_value );
  comp.GetProcessor().SetCoreCount( d.computer.processor.core_count );
  comp.GetProcessor().SetCoreSpeed( d.computer.processor.core_speed );

  // update the player's OS details
  comp.GetOS().SetItemInfo( d.computer.os.item_id, d.computer.os.name, d.computer.os.itemtype_id,
                            d.computer.os.min_level, d.computer.os.max_level, d.computer.os.npc_value );
  comp.GetOS().SetKernelID( d.computer.os.kernel_id );
  comp.GetOS().SetKernelName( d.computer.os.kernel_name );

  // update the player's memory details
  comp.GetMemory().SetItemInfo( d.computer.memory.item_id, d.computer.memory.name, d.computer.memory.itemtype_id,
                                d.computer.memory.min_level, d.computer.memory.max_level, d.computer.memory.npc_value );
  comp.GetMemory().SetMemorySize( d.computer.memory.mem_size );

  // update the player's network port details
  NetworkPorts& ports = comp.GetNetworkPorts();
  FCSHORT portnum;
  for ( int i = 0; i < d.computer.availablePorts; i++ )
  {
    portnum = ports.addPort();
    ports.installPort(portnum, d.computer.network_ports[i].softwareType, d.computer.network_ports[i].itemID);
    if ( d.computer.network_ports[i].enabled )
    {
      ItemMgr::GameItem gi;

      ports.enablePort(portnum);
      if ( itemMgr.getItem( d.computer.network_ports[i].itemID, gi ) )
      {
        comp.AddProcess( static_cast<ItemSoftware*>(gi.getItem()) );
      }
    }
    ports.setPortHealth(portnum, d.computer.network_ports[i].portHealth);
    ports.setPortMaxHealth(portnum, d.computer.network_ports[i].portMaxHealth);
  }

  m_pApp->network().requestCharacterMissions( m_pApp->playerModel()->getCurrentCharacter()->GetID() );

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseCharacterMissionsRequest(PEPacket* pPkt)
{
  __FCPKT_CHARACTER_MISSIONS_REQUEST_RESP* d = NULL;
  FCMissionMgr& missionMgr = FCAPP->missionMgr();

  getDynamicPacketData<__FCPKT_CHARACTER_MISSIONS_REQUEST_RESP>(pPkt, d);

  for ( FCULONG i = 0; i < d->numMissions; i++ )
  {
    missionMgr.addMission( d->missions[i].mission_id, d->missions[i].success_count, d->missions[i].failure_count, d->missions[i].completed, d->missions[i].parent_id );
  }

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

void PacketHandler::onError(PEPacket* pPkt)
{
  if ( !pPkt )
    return;
}
