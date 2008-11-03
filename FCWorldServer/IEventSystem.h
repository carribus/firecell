#ifndef _IEVENTSYSTEM_H_
#define _IEVENTSYSTEM_H_

/*
 *  IEventSource represents the source object of a specific event.
 */
struct IEventSource
{
};

/*
 *  IEventTarget represents the destination or target object of a specific event.
 */
struct IEventTarget
{
};

/*
 *  IEvent represents an event object
 */
struct IEvent
{
};

struct IEventSystem
{
  virtual void Emit(IEventSource* source, IEventTarget* target, IEvent* event) = 0;
};

#endif//_IEVENTSYSTEM_H_