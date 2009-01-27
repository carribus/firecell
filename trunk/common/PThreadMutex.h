#ifndef _PTHREADMUTEX_H_
#define _PTHREADMUTEX_H_

#ifdef _WIN32
  #include "pthreads-win32/include/pthread.h"
  #include "pthreads-win32/include/sched.h"
#else
  #include <pthread.h>
  #include <sched.h>
#endif

class PThreadMutex
{
public:
	PThreadMutex(void);
	~PThreadMutex(void);

	void Lock();
	void Unlock();

private:

	pthread_mutex_t					m_mutex;
};

#endif//_PTHREADMUTEX_H_