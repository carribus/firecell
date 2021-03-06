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
#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

#include <map>
#include "../common/game_objects/PlayerManagerBase.h"
#include "Player.h"

class PlayerManager : public PlayerManagerBase< Player >
{
  typedef PlayerManagerBase< Player > _super;

  PlayerManager();
  ~PlayerManager(void);
public:

  static PlayerManager& instance();
  static void destroy();

  Player* CreatePlayer(FCULONG accountID, FCULONG id, string name, FCULONG xp, FCULONG level, FCINT fame_scale, FCULONG country_id, FCULONG city_id, FCSOCKET clientSocket);

  void SetEventSystem(IEventSystem* pES)                { m_pEventSystem = pES; }

private:

  static PlayerManager* m_pThis;

  IEventSystem* m_pEventSystem;
};

#endif//_PLAYERMANAGER_H_