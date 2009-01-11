#ifndef _PTHREADMUTEX_H_
#define _PTHREADMUTEX_H_

#ifdef _WIN32
  #include "../../common/pthreads-win32/include/pthread.h"
  #include "../../common/pthreads-win32/include/sched.h"
  // include the the Visual Studio compatible Structured Exception handling version of the pthreads-win32 library
  #pragma comment(lib, "../../common/pthreads-win32/lib/pthreadVSE2.lib")
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