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
#ifndef _IEVENTSYSTEM_H_
#define _IEVENTSYSTEM_H_

#include <string>
#include "../common/fctypes.h"

#define DECLARE_EVENT_SOURCE()  \
  static const string EVTSYS_ObjectType;

#define DEFINE_EVENT_SOURCE(objectType) \
  const string objectType::EVTSYS_ObjectType = #objectType;

#define DECLARE_EVENT(eventName) \
  static const string EVT_##eventName;

#define DEFINE_EVENT(objectType, eventName) \
  const string objectType::EVT_##eventName = #objectType "." #eventName;

/*
 *  Forward declarations
 */
struct IEventSystem;

/*
 *  IEvent represents an event object
 */
struct IEvent
{
  virtual std::string GetCode() = 0;
  virtual void* GetParam() = 0;
  virtual void Release() = 0;
};

/*
 *  IEventSource represents the source object of a specific event.
 */
struct IEventSource
{
  virtual const std::string& GetType() = 0;
};

/*
 *  IEventTarget represents the destination or target object of a specific event.
 */
struct IEventTarget
{
  virtual void RegisterForEvents(IEventSystem*) = 0;
  virtual void OnEvent(IEventSource*, IEvent*) = 0;
  virtual const std::string& GetType() = 0;
};

struct IEventSystem
{
  virtual void Emit(IEventSource* source, IEventTarget* target, IEvent* event) = 0;
  virtual void RegisterEventTarget(IEventTarget* pTarget, const std::string& eventCode) = 0;
};

#endif//_IEVENTSYSTEM_H_