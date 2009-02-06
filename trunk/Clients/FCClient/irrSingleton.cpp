#include "irrSingleton.h"

irrSingleton* irrSingleton::m_pThis = NULL;

irrSingleton::irrSingleton(void)
: m_pDevice(NULL)
, m_pTimer(NULL)
{
}

irrSingleton::~irrSingleton(void)
{
}

irrSingleton& irrSingleton::instance()
{
	if ( !m_pThis )
	{
		m_pThis = new irrSingleton;
	}
	return *m_pThis;
}

void irrSingleton::destroy()
{
	if ( m_pThis )
	{
		delete m_pThis;
		m_pThis = NULL;
	}
}
