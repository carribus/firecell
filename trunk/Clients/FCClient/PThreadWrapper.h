#ifndef _PTHREADWRAPPER_H_
#define _PTHREADWRAPPER_H_

#ifdef _WIN32
  #include "../../common/pthreads-win32/include/pthread.h"
  #include "../../common/pthreads-win32/include/sched.h"
  // include the the Visual Studio compatible Structured Exception handling version of the pthreads-win32 library
  #pragma comment(lib, "../../common/pthreads-win32/lib/pthreadVSE2.lib")
#else
  #include <pthread.h>
  #include <sched.h>
#endif

// macro to enable base template class to access a protected/private RunThread function on the templated parameter object
#define DECLARE_THREAD_CLASS(className) \
	friend class PThreadWrapper< className >

template <class T>
class PThreadWrapper
{
public:

	PThreadWrapper(void)
		: m_bRunning(false)
	{
	}

	virtual ~PThreadWrapper(void)
	{
		Stop();
	}

	bool Start(long long data)
	{
		m_data = data;
		if ( pthread_create( &m_thread, NULL, T::RunThread, (void*)this ) != 0 )
		{
			return false;
		}

		m_bRunning = true;

		return true;
	}

	void Stop()
	{
    if ( m_bRunning )
    {
		  m_bRunning = false;
		  pthread_join(m_thread, 0);
    }
	}

	long long GetData()
	{
		return m_data; 
	}

	bool IsRunning()													{ return m_bRunning; }

private:

	pthread_t			m_thread;
	long long			m_data;
	bool					m_bRunning;
};

#endif//_PTHREADWRAPPER_H_