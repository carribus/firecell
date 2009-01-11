#include "PThreadMutex.h"

PThreadMutex::PThreadMutex(void)
{
	pthread_mutex_init(&m_mutex, NULL);
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
