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
#include "FCApp.h"
#include "FCPlayerModel.h"
#include "FCForumModel.h"
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

  case  FCMSG_SOFTWARE_INSTALL:
    {
      bHandled = onResponseSoftwareInstallRequest(pPkt);
    }
    break;

  case  FCMSG_SOFTWARE_UNINSTALL:
    {
      bHandled = onResponseSoftwareUninstallRequest(pPkt);
    }
    break;

  case  FCMSG_SOFTWARE_NETWORK_PORT_ENABLE:
    {
      bHandled = onResponseSoftwareNetworkPortEnableRequest(pPkt);
    }
    break;

  case  FCMSG_ACTIVATE_SOFTWARE:
    {
      bHandled = onResponseActivateSoftware(pPkt);
    }
    break;

  case  FCMSG_FORUM_GET_CATEGORIES:
    {
      bHandled = onResponseForumCategories(pPkt);
    }
    break;

  case  FCMSG_FORUM_GET_THREADS:
    {
      bHandled = onResponseForumThreads(pPkt);
    }

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

  getDynamicPacketData<__FCPKT_CHARACTER_ITEMS_REQUEST_RESP>(pPkt, d);

  if ( !d )
    return false;

  m_pApp->playerModel()->addItems(d);

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
  const std::string procName = d.computer.processor.name;
  comp.GetProcessor().SetItemInfo( d.computer.processor.item_id, procName, d.computer.processor.itemtype_id,
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
  FCMissionMgr& missionMgr = m_pApp->missionMgr();

  getDynamicPacketData<__FCPKT_CHARACTER_MISSIONS_REQUEST_RESP>(pPkt, d);

  for ( FCULONG i = 0; i < d->numMissions; i++ )
  {
    missionMgr.addMission( d->missions[i].mission_id, d->missions[i].success_count, d->missions[i].failure_count, d->missions[i].completed, d->missions[i].parent_id );
  }

  delete [] (FCBYTE*)d;

  // update the state of the model
  FCAPP->setState( AppStatePlaying );

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseSoftwareInstallRequest(PEPacket* pPkt)
{
  __FCPKT_SOFTWARE_INSTALL_RESP d;
  FCPlayerModel* pPlayer = m_pApp->playerModel();

  getPacketData<__FCPKT_SOFTWARE_INSTALL_RESP>(pPkt, d);

  QMetaObject::invokeMethod(pPlayer, 
                            "onSoftwareInstallResult", 
                            Qt::QueuedConnection, 
                            Q_ARG(bool, d.bResult),
                            Q_ARG(FCSHORT, d.portNum),
                            Q_ARG(FCULONG, d.itemID));

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseSoftwareUninstallRequest(PEPacket* pPkt)
{
  __FCPKT_SOFTWARE_UNINSTALL_RESP d;
  FCPlayerModel* pPlayer = m_pApp->playerModel();

  getPacketData<__FCPKT_SOFTWARE_UNINSTALL_RESP>(pPkt, d);

  QMetaObject::invokeMethod(pPlayer, 
                            "onSoftwareUninstallResult", 
                            Qt::QueuedConnection, 
                            Q_ARG(bool, d.bResult),
                            Q_ARG(FCSHORT, d.portNum));

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseSoftwareNetworkPortEnableRequest(PEPacket* pPkt)
{
  __FCPKT_SOFTWARE_NETWORK_PORT_ENABLE_RESP d;
  FCPlayerModel* pPlayer = m_pApp->playerModel();

  getPacketData<__FCPKT_SOFTWARE_NETWORK_PORT_ENABLE_RESP>(pPkt, d);

  QMetaObject::invokeMethod(pPlayer,
                            "onNetworkPortStatusChangeResult",
                            Qt::QueuedConnection,
                            Q_ARG(FCSHORT, d.result),
                            Q_ARG(bool, d.bEnabled),
                            Q_ARG(FCSHORT, d.portNum));

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseActivateSoftware(PEPacket* pPkt)
{
  __FCPKT_ACTIVATE_SOFTWARE_RESP d;
  FCPlayerModel* pPlayer = m_pApp->playerModel();

  getPacketData<__FCPKT_ACTIVATE_SOFTWARE_RESP>(pPkt, d);

  QMetaObject::invokeMethod(pPlayer,
                            "onActivateSoftwareResult",
                            Qt::QueuedConnection,
                            Q_ARG(FCULONG, d.result),
                            Q_ARG(FCULONG, d.itemID),
                            Q_ARG(FCSHORT, d.cpu_cost),
                            Q_ARG(FCULONG, d.mem_cost));


  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseForumCategories(PEPacket* pPkt)
{
  __FCPKT_FORUM_GET_CATEGORIES_RESP* d = NULL;
  FCForumModel* forumModel = FCAPP->forumModel();

  getDynamicPacketData<__FCPKT_FORUM_GET_CATEGORIES_RESP>(pPkt, d);

  if ( !d )
    return false;

  for ( FCSHORT i = 0; i < d->category_count; i++ )
  {
    forumModel->addCategory( d->categories[i].category_id,
                             d->categories[i].parent_id,
                             d->categories[i].order,
                             d->categories[i].thread_count,
                             d->categories[i].name,
                             d->categories[i].desc );
  }

  delete [] (FCBYTE*)d;

  return true;
}

///////////////////////////////////////////////////////////////////////

bool PacketHandler::onResponseForumThreads(PEPacket* pPkt)
{
  __FCPKT_FORUM_GET_THREADS_RESP* d = NULL;

  // TODO: Need to create a new class call FCForumThreadModel to act as the model for the data that we receive on this response.

  getDynamicPacketData<__FCPKT_FORUM_GET_THREADS_RESP>(pPkt, d);

  if ( !d )
    return false;

  for ( FCULONG i = 0; i < d->thread_count; i++ )
  {
    // populate the FCForumThreadModel object
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
