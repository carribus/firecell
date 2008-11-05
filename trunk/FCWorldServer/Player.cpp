/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

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
#include "Player.h"

DEFINE_EVENT_SOURCE(Player);
DEFINE_EVENT(Player, LoggedIn);
DEFINE_EVENT(Player, LoggedOut);

Player::Player(void)
: m_accountID(0)
, m_id(0)
, m_xp(0)
, m_level(0)
, m_fameScale(0)
, m_cityID(0)
, m_countryID(0)
, m_clientSocket(0)
, m_pEventSystem(NULL)
{
  printf("Player Object constructing -- ID:%ld, name=%ld, address=%p\n", m_id, m_name, this);
}

///////////////////////////////////////////////////////////////////////

Player::Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID, InGameIPAddress* ip)
: m_accountID(accountID)
, m_id(id)
, m_name(name)
, m_email(email)
, m_xp(xp)
, m_level(level)
, m_fameScale(fameScale)
, m_cityID(cityID)
, m_countryID(countryID)
, m_clientSocket(0)
, m_pEventSystem(NULL)
{
  if ( ip )
    m_ip = *ip;
}

///////////////////////////////////////////////////////////////////////

Player::Player(const Player& src)
{
  *this = src;
}

///////////////////////////////////////////////////////////////////////

Player::~Player(void)
{
  if ( m_pEventSystem )
  {
    m_pEventSystem->UnregisterEventTarget(this, Player::EVT_LoggedIn);
    m_pEventSystem->UnregisterEventTarget(this, Player::EVT_LoggedOut);
  }
}

///////////////////////////////////////////////////////////////////////

void Player::RegisterForEvents(IEventSystem* pEventSystem)
{
  if ( !pEventSystem )
    return;

  m_pEventSystem = pEventSystem;

  // test listener
  pEventSystem->RegisterEventTarget(this, Player::EVT_LoggedIn);
  pEventSystem->RegisterEventTarget(this, Player::EVT_LoggedOut);
}

///////////////////////////////////////////////////////////////////////

void Player::OnEvent(IEventSource* pSource, IEvent* pEvent)
{
  string sourceType = (pSource ? pSource->GetType() : "World");
  string eventCode = pEvent->GetCode();
  string eventName = eventCode.substr(eventCode.find('.')+1, eventCode.length());

  printf("Event [%s] received from source [%s]\n", eventCode.c_str(), sourceType.c_str());

  if ( !sourceType.compare( Player::EVTSYS_ObjectType ) )      // player events
  {
    Player* pPlayer = (Player*)pEvent->GetParam();

    // ensure that we do not handle our own event
    if ( pSource != (IEventSource*)this )
    {
      // handle events
      if ( !eventCode.compare( Player::EVT_LoggedIn ) )
      {
      }
    }
  }
  else
  {
    printf("Player::OnEvent(): Unknown event received [source:%s / event:%s]\n", sourceType.c_str(), pEvent->GetCode().c_str());
  }


}
