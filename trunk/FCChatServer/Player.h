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
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <map>
#include <string>
#include <vector>
#include "../common/fctypes.h"
#include "../common/PThreadRWLock.h"
#include "../Clients/common/Socket/ClientSocket.h"
#include "../common/game_objects/PlayerBase.h"

using namespace std;

class Player : public PlayerBase
{
public:

  Player(void);
  Player(FCULONG accountID, FCULONG id, string name, string email, FCULONG xp, FCULONG level, FCINT fameScale, FCULONG cityID, FCULONG countryID);
  ~Player(void);

private:

};

#endif//_PLAYER_H_