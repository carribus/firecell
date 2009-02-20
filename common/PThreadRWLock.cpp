#include "PThreadRWLock.h"

PThreadRWLock::PThreadRWLock(void)
{
  pthread_rwlock_init(&m_lock, NULL);
}

///////////////////////////////////////////////////////////////////////

PThreadRWLock::~PThreadRWLock(void)
{
  pthread_rwlock_destroy(&m_lock);
}

///////////////////////////////////////////////////////////////////////

void PThreadRWLock::LockForRead()
{
  pthread_rwlock_rdlock(&m_lock);
}

///////////////////////////////////////////////////////////////////////

void PThreadRWLock::LockForWrite()
{
  pthread_rwlock_wrlock(&m_lock);
}

///////////////////////////////////////////////////////////////////////

void PThreadRWLock::Unlock()
{
  pthread_rwlock_unlock(&m_lock);
}
