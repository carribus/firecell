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
#ifdef _WIN32
  #include <windows.h>
#endif//_WIN32
#include "../common/Logging/DynLog.h"
#include "EventSystem.h"

EventSystem* EventSystem::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

EventSystem::EventSystem(void)
: m_bThrdRunning(false)
{
  pthread_mutex_init(&m_mutexQueue, NULL);
}

///////////////////////////////////////////////////////////////////////

EventSystem::~EventSystem(void)
{
  pthread_mutex_destroy(&m_mutexQueue);
}

///////////////////////////////////////////////////////////////////////

EventSystem* EventSystem::GetInstance()
{
  if ( !m_pThis )
  {
    m_pThis = new EventSystem;
  }

  return m_pThis;
}

///////////////////////////////////////////////////////////////////////

void EventSystem::Shutdown()
{
  if ( m_pThis )
  {
    if ( m_pThis->m_bThrdRunning )
    {
      m_pThis->m_bThrdRunning = false;
      pthread_join(m_pThis->m_thrdEventMgr, 0);
    }
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

bool EventSystem::Start()
{
  DYNLOG_ADDLOG("Event System starting up");
  // kick off the eventing thread
  if ( pthread_create( &m_thrdEventMgr, NULL, thrdEventMgr, (void*)this ) != 0 )
  {
    DYNLOG_ADDLOG("Event System startup aborted - failed to create thread");
    return false;
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

void EventSystem::Emit(IEventSource* source, IEventTarget* target, IEvent* event)
{
  InternalEvent e = { source, target, event };

  LockEventQueue();
  m_eventQueue.push(e);
  UnlockEventQueue();
}

///////////////////////////////////////////////////////////////////////

void EventSystem::RegisterEventTarget(IEventTarget* pTarget, const std::string& eventCode)
{
  if ( !pTarget || eventCode.empty() )
    return;

  m_mapEventCodeTargets[eventCode].WriteLock();
  m_mapEventCodeTargets[eventCode].m_targets.push_back(pTarget);
  m_mapEventCodeTargets[eventCode].Unlock();
}

///////////////////////////////////////////////////////////////////////

void EventSystem::UnregisterEventTarget(IEventTarget* pTarget, const std::string& eventCode)
{
  if ( !pTarget || eventCode.empty() )
    return;

  m_mapEventCodeTargets[eventCode].WriteLock();
  for ( TargetArray::iterator it = m_mapEventCodeTargets[eventCode].m_targets.begin(); it != m_mapEventCodeTargets[eventCode].m_targets.end(); it++ )
  {
    if ( (*it) == pTarget )
    {
      m_mapEventCodeTargets[eventCode].m_targets.erase(it);
      break;
    }
  }
  m_mapEventCodeTargets[eventCode].Unlock();
}

///////////////////////////////////////////////////////////////////////

void EventSystem::LockEventQueue()
{
  pthread_mutex_lock(&m_mutexQueue);
}

///////////////////////////////////////////////////////////////////////

void EventSystem::UnlockEventQueue()
{
  pthread_mutex_unlock(&m_mutexQueue);
}

///////////////////////////////////////////////////////////////////////
//  ProcessEvent(InternalEvent& ev)
//
//  This function is responsible for identifying all the targets
//  for an event (ev.pEvent) and signalling them with the event
///////////////////////////////////////////////////////////////////////
void EventSystem::ProcessEvent(InternalEvent& ev)
{
  string evCode = ev.pEvent->GetCode();

  // check if the event has been targetted
  if ( ev.pTarget )
  {
    ev.pTarget->OnEvent( ev.pSource, ev.pEvent );
  }
  else
  {
    SafeTargetArray& targets = m_mapEventCodeTargets[evCode];

    targets.ReadLock();
    // if no specific targetting was done, we find the objects that are registered for this event
    for ( TargetArray::iterator it = targets.m_targets.begin(); it != targets.m_targets.end(); it++ )
    {
      (*it)->OnEvent(ev.pSource, ev.pEvent);
    }
    targets.Unlock();
  }
}

///////////////////////////////////////////////////////////////////////

void* EventSystem::thrdEventMgr(void* pData)
{
  EventSystem* pThis = (EventSystem*) pData;
  queue<InternalEvent>* pQueue = &pThis->m_eventQueue;
  InternalEvent e;

  pThis->m_bThrdRunning = true;
  while ( pThis->m_bThrdRunning )
  {
    // do all event signalling here
    while ( !pQueue->empty() )
    {
      // get the next event
      e = pQueue->front();
      
      pThis->ProcessEvent(e);

      // free the event
      e.pEvent->Release();
      
      // remove the event from the queue
      pThis->LockEventQueue();
      pQueue->pop();
      pThis->UnlockEventQueue();
    }

#ifdef _WIN32
    Sleep(250);
#else
    usleep(250000);
#endif
  }

  return 0;
}