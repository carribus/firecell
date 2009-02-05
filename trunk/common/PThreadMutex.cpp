#include "PThreadMutex.h"

PThreadMutex::PThreadMutex(bool bRecursive)
{
	pthread_mutexattr_t attr;

	if ( bRecursive )
	{
	  pthread_mutexattr_init(&attr);
	  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
	}

	pthread_mutex_init(&m_mutex, (bRecursive ? &attr : NULL) );

	if ( bRecursive )
		pthread_mutexattr_destroy(&attr);
}

///////////////////////////////////////////////////////////////////////

PThreadMutex::~PThreadMutex(void)
{
	pthread_mutex_destroy(&m_mutex);
}

///////////////////////////////////////////////////////////////////////

void PThreadMutex::Lock()
{
	pthread_mutex_lock(&m_mutex);
}

///////////////////////////////////////////////////////////////////////

void PThreadMutex::Unlock()
{
	pthread_mutex_unlock(&m_mutex);
}
