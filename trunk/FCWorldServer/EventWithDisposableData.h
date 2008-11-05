#ifndef _EVENTWITHDISPOSABLEDATA_H_
#define _EVENTWITHDISPOSABLEDATA_H_

#include "IEventSystem.h"

template <class T>
class EventWithDisposableData : public IEvent
{
public:

  EventWithDisposableData(string code, T* param)
    : m_code(code)
    , m_param(param)
  {
  }

  virtual ~EventWithDisposableData(void)
  {
    delete m_param;
  }

  /*
   *  IEvent implementation
   */
  string GetCode()                    { return m_code; }
  void* GetParam()                    { return m_param; }
  void Release()                      { delete this; }

private:

  string m_code;
  T* m_param;

};

#endif//_EVENTWITHDISPOSABLEDATA_H_