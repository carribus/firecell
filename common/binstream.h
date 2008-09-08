/*
    FireCell Server - The server code for the firecell multiplayer game
    Copyright (C) 2008  Peter M. Mares

		Contact: carribus@gmail.com

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _BINSTREAM_H_
#define _BINSTREAM_H_

#include <memory.h>
#include "threading.h"

template <typename T, bool bThreadSafe = false>
class CBinStream
{
public:
	CBinStream(void)
		: m_pData(NULL)
		, m_ulLen(0)
	{
		if ( bThreadSafe )
    {
      pthread_mutexattr_t attr;
      
      // define the mutex as a recursive mutex
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
      // initialize the recursive mutex
      pthread_mutex_init(&m_mutex, &attr);
      // destroy the mutex attributes object
      pthread_mutexattr_destroy(&attr);
    }
	}

	//////////////////////////////////////////////////////////////////////////////////////

	CBinStream(CBinStream<T, bThreadSafe>& s)
		: m_pData(NULL)
		, m_ulLen(0)
	{
		if ( bThreadSafe )
    {
      pthread_mutexattr_t attr;
      
      // define the mutex as a recursive mutex
      pthread_mutexattr_init(&attr);
      pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
      // initialize the recursive mutex
      pthread_mutex_init(&m_mutex, &attr);
      // destroy the mutex attributes object
      pthread_mutexattr_destroy(&attr);
    }
		Assign(s.m_pData, s.m_ulLen);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	virtual ~CBinStream(void)
	{
		Empty();
		if ( bThreadSafe )
    {
      pthread_mutex_destroy(&m_mutex);
    }
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// PUBLIC: Methods
	//////////////////////////////////////////////////////////////////////////////////////

	void Empty()
	{
		Lock();
		if ( m_pData )
		{
			delete [] m_pData;
			m_ulLen = 0;
			m_pData = NULL;
		}
		Unlock();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void Concat(T* pData, unsigned long ulLen)
	{
		if ( !pData || !ulLen )
			return;

		Lock();
		if ( !m_pData )
		{
			Assign(pData, ulLen);
		}
		else
		{
			unsigned long	ulNewLen = m_ulLen+ulLen;
			T*				pNew = new T[ulNewLen];

			if ( pNew )
			{
				memcpy(pNew, m_pData, m_ulLen);
				memcpy(pNew+m_ulLen, pData, ulLen);
				m_ulLen = ulNewLen;
				delete [] m_pData;
				m_pData = pNew;
			}
		}
		Unlock();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	long Find(T* pData, unsigned long ulLen, unsigned long ulStartAt = 0)
	{
		if ( !pData || ulStartAt >= m_ulLen )
			return -1;

		unsigned long				i;

		Lock();
		for ( i = ulStartAt; i < m_ulLen; i++ )
		{
			if ( m_pData[i] == pData[0] )
			{
				// we have found the first matching character of the search string
				if ( !_memicmp(&m_pData[i], pData, ulLen) )
				{
					Unlock();
					return (signed)i;
				}
			}
		}
		Unlock();
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	CBinStream<T, bThreadSafe>& Mid(unsigned long ulStartAt, unsigned long ulCount)
	{
		static CBinStream<T, bThreadSafe>			stream;

		Lock();
		stream.Empty();
		if ( ulStartAt+ulCount >= m_ulLen )
			ulCount = m_ulLen - ulStartAt;
		stream.Assign( m_pData+ulStartAt, ulCount );
		Unlock();

		return stream;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void Delete(unsigned long ulIndex, unsigned long ulCount = 1)
	{
		T*					pNewData = NULL;
		unsigned long		ulNewBase = ulIndex + ulCount;

		if ( ulNewBase > m_ulLen )
			return;

		if ( m_ulLen-ulCount == 0 )
		{
			Empty();
			return;
		}
		Lock();
		pNewData = new T[m_ulLen-ulCount];
		memcpy(pNewData, m_pData+ulIndex+ulCount, m_ulLen-ulCount);
		delete [] m_pData;
		m_pData = pNewData;
		m_ulLen -= ulCount;
		Unlock();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	unsigned long GetLength()
	{
		return m_ulLen; 
	}

	//////////////////////////////////////////////////////////////////////////////////////

	CBinStream<T, bThreadSafe>& operator+=(CBinStream<T, bThreadSafe>& s)
	{
		Concat(s.m_pData, s.m_ulLen);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	CBinStream<T, bThreadSafe>& operator=(CBinStream<T, bThreadSafe>& s)
	{
		Assign(s.m_pData, s.m_ulLen);

		return *this;
	}

	//////////////////////////////////////////////////////////////////////////////////////

	operator T*()
	{
		return m_pData;
	}

protected:

	//////////////////////////////////////////////////////////////////////////////////////
	// PROTECTED: Methods
	//////////////////////////////////////////////////////////////////////////////////////

	void Assign(T* pData, unsigned long ulLen)
	{
		Lock();
		Empty();
		if ( !pData || !ulLen )
		{
			Unlock();
			return;
		}
		if ( (m_pData = new T[ulLen]) )
		{
			memcpy(m_pData, pData, sizeof(T)*ulLen);
			m_ulLen = ulLen;
		}
		Unlock();
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void Lock()
	{
		if ( bThreadSafe )
      pthread_mutex_lock(&m_mutex);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	void Unlock()
	{
		if ( bThreadSafe )
			pthread_mutex_unlock(&m_mutex);
	}

	//////////////////////////////////////////////////////////////////////////////////////
	// PROTECTED: Attributes
	//////////////////////////////////////////////////////////////////////////////////////
	T*					        m_pData;
	unsigned long		    m_ulLen;
  pthread_mutex_t     m_mutex;
};

#endif//_BINSTREAM_H_