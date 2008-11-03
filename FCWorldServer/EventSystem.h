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
#ifndef _EVENTSYSTEM_H_
#define _EVENTSYSTEM_H_

#include "../common/threading.h"
#include "IEventSystem.h"

class EventSystem : public IEventSystem
{
  EventSystem(void);
  ~EventSystem(void);

public:

  static EventSystem* GetInstance();
  static void Shutdown();

  bool Start();

  /*
   *  IEventSystem implementation
   */
  void Emit(IEventSource* source, IEventTarget* target, IEvent* event);

private:

  /*
   *  Private methods
   */
  static void* thrdEventMgr(void* pData);

  /*
   *  Private members
   */
  static EventSystem* m_pThis;

  pthread_t m_thrdEventMgr;
  bool m_bThrdRunning;

};

#endif//_EVENTSYSTEM_H_