#ifndef _PTHREADCOND_H_
#define _PTHREADCOND_H_

#ifdef _WIN32
  #include "pthreads-win32/include/pthread.h"
  #include "pthreads-win32/include/sched.h"
#else
  #include <pthread.h>
  #include <sched.h>
#endif

class PThreadCond
{
public:
  PThreadCond(void);
  virtual ~PThreadCond(void);

  void WaitForSignal();
  void Signal();
  void BroadcastSignal();

protected:

  pthread_cond_t        m_cond;
  pthread_mutex_t       m_mutex;
};

#endif//_PTHREADCOND_H_