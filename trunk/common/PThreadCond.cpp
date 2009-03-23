#include "PThreadCond.h"

PThreadCond::PThreadCond(void)
{
  pthread_cond_init(&m_cond, NULL);
  pthread_mutex_init(&m_mutex, NULL);
}

/////////////////////////////////////////////////////////////////////////////////

PThreadCond::~PThreadCond(void)
{
  pthread_cond_destroy(&m_cond);
  pthread_mutex_destroy(&m_mutex);
}

/////////////////////////////////////////////////////////////////////////////////

void PThreadCond::WaitForSignal()
{
  pthread_mutex_lock(&m_mutex);
  pthread_cond_wait(&m_cond, &m_mutex);
  pthread_mutex_unlock(&m_mutex);
}

/////////////////////////////////////////////////////////////////////////////////

void PThreadCond::Signal()
{
  pthread_cond_signal(&m_cond);
}

/////////////////////////////////////////////////////////////////////////////////

void PThreadCond::BroadcastSignal()
{
  pthread_cond_broadcast(&m_cond);
}