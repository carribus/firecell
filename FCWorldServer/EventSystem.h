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

#include <map>
#include <queue>
#include <string>
#include "../common/threading.h"
#include "IEventSystem.h"

using namespace std;

class EventSystem : public IEventSystem
{
  EventSystem(void);
  ~EventSystem(void);

  struct InternalEvent
  {
    IEventSource* pSource;
    IEventTarget* pTarget;
    IEvent* pEvent;
  };

public:

  static EventSystem* GetInstance();
  static void Shutdown();

  bool Start();

  /*
   *  IEventSystem implementation
   */
  void Emit(IEventSource* source, IEventTarget* target, IEvent* event);
  void RegisterEventTarget(IEventTarget* pTarget, const string& eventCode);
  void UnregisterEventTarget(IEventTarget* pTarget, const std::string& eventCode);

private:

  /*
   *  Private methods
   */
  void LockEventQueue();
  void UnlockEventQueue();

  void ProcessEvent(InternalEvent& ev);

  static void* thrdEventMgr(void* pData);

  /*
   *  Private members
   */
  static EventSystem* m_pThis;

  /*
   *  Event pumping thread
   */
  pthread_t m_thrdEventMgr;
  bool m_bThrdRunning;

  /*
   *  Event queueing mechanism
   */
  queue<InternalEvent> m_eventQueue;
  pthread_mutex_t m_mutexQueue;

  /*
   *  EventTarget pools
   */
  typedef vector<IEventTarget*> TargetArray;
  class SafeTargetArray
  {
  public:
    SafeTargetArray()
    {
      pthread_rwlock_init(&m_lock, NULL);
    }

    ~SafeTargetArray()
    {
      pthread_rwlock_destroy(&m_lock);
    }

    void ReadLock()
    {
      pthread_rwlock_rdlock(&m_lock);
    }

    void Unlock()
    {
      pthread_rwlock_unlock(&m_lock);
    }

    void WriteLock()
    {
      pthread_rwlock_wrlock(&m_lock);
    }

    TargetArray m_targets;
    pthread_rwlock_t m_lock;
  };
  typedef map<string, SafeTargetArray> EventCodeTargetMap;
  EventCodeTargetMap m_mapEventCodeTargets;
};

#endif//_EVENTSYSTEM_H_