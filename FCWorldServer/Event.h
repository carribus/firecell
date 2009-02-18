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
#ifndef _EVENT_H_
#define _EVENT_H_

#include <string>
#include "Player.h"
#include "IEventSystem.h"

using namespace std;

class Event : public IEvent
{
public:
  Event(string code, void* param, Player* pPlayer = NULL);
  ~Event(void);

  /*
   *  IEvent implementation
   */
  string GetCode()                    { return m_code; }
  void* GetParam()                    { return m_param; }
	Player* GetPlayer()									{ return m_pPlayer; }
  void Release()                      { delete this; }

  /*
   *  Public methods
   */

private:

  string m_code;
  void* m_param;
	Player* m_pPlayer;
};

#endif//_EVENT_H_