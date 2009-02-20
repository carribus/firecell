#ifndef _PTHREADRWLOCK_H_
#define _PTHREADRWLOCK_H_

#ifdef _WIN32
  #include "pthreads-win32/include/pthread.h"
  #include "pthreads-win32/include/sched.h"
#else
  #include <pthread.h>
  #include <sched.h>
#endif

class PThreadRWLock
{
public:
  PThreadRWLock(void);
  virtual ~PThreadRWLock(void);

  void LockForRead();
  void LockForWrite();
  void Unlock();

private:

  pthread_rwlock_t      m_lock;
};

#endif//_PTHREADRWLOCK_H_