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
#if !defined(_DATAMGR_H_)
#define _DATAMGR_H_

#ifdef _WIN32
#include <windows.h>
#endif//_WIN32

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class Type, bool bManaged = true>
class CDataMgr  
{
public:
	CDataMgr()
	{
		m_nNumItems = 0;
		m_ppData = NULL;
#ifdef _WIN32
		InitializeCriticalSection(&m_csLock);
#endif
	}

	//////////////////////////////////////////////////////////////////

	virtual ~CDataMgr()
	{
		if ( bManaged )
			DeleteAll();
		else
			RemoveAll();

#ifdef _WIN32
		DeleteCriticalSection(&m_csLock);
#endif
	}


	//
	// methods
	virtual int	Add(Type* pData)
	{
		Lock();
		if ( !m_ppData )
		{
			m_ppData = new Type*[1];
			memset(m_ppData, 0, sizeof(Type*) * (m_nNumItems+1));
		}
		else
		{
			Type**				ppNew = NULL;

			ppNew = new Type*[m_nNumItems+1];
			memset(ppNew, 0, sizeof(Type*) * (m_nNumItems+1));
			memcpy(ppNew, m_ppData, m_nNumItems * sizeof(Type*));
			delete [] m_ppData;
			m_ppData = ppNew;
		}

		// add the new item to the manager
		m_ppData[m_nNumItems] = pData;
		Unlock();

		return (++m_nNumItems);
	}

	//////////////////////////////////////////////////////////////////

	virtual int	InsertAt ( int nIndex, Type* pData)
	{
		if ( nIndex == -1 )
			return ( Add ( pData ) - 1 );

		if (( nIndex < 0 ) || ( nIndex > m_nNumItems ))
			return ( -1 );

		Lock();
		if (( !m_ppData ) && ( nIndex == 0 ))
		{
			m_ppData = new Type*[1];
			memset(m_ppData, 0, sizeof(Type*) * (m_nNumItems+1));
			m_nNumItems = 1;
		}
		else
		{
			if ( !m_ppData )
				return ( -1 );

			Type**				ppNew = NULL;

			ppNew = new Type*[m_nNumItems+1];
			memset ( ppNew, 0, sizeof(Type*) * (m_nNumItems+1) );
			memcpy ( ppNew, m_ppData, nIndex * sizeof(Type*) );
			memcpy ( ( ppNew + nIndex + 1 ), ( m_ppData + nIndex ), ( m_nNumItems - nIndex ) * sizeof(Type*));
			delete [] m_ppData;
			m_ppData = ppNew;
			m_nNumItems++;
		}

		// add the new item to the manager
		if (( m_ppData ) && ( nIndex >= 0 ))
		{
			m_ppData[nIndex] = pData;
			Unlock();
			return (nIndex);
		}
		Unlock();

		return ( -1 );
	}

	//////////////////////////////////////////////////////////////////

	virtual bool Delete(int nIndex)
	{
		if ( nIndex < 0 || nIndex >= m_nNumItems )
			return false;

		Type**				ppNew = NULL;

		Lock();
		delete m_ppData[nIndex];
		ppNew = new Type*[m_nNumItems-1];
		memset(ppNew, 0, sizeof(Type*) * (m_nNumItems-1));
		memcpy(ppNew, m_ppData, nIndex * sizeof(Type*));
		memcpy(&ppNew[nIndex], &m_ppData[nIndex+1], (m_nNumItems-nIndex-1) * sizeof(Type*));
		delete [] m_ppData;
		m_ppData = ppNew;
		m_nNumItems--;
		Unlock();

		return true;	
	}

	//////////////////////////////////////////////////////////////////

	virtual void DeleteAll()
	{
		Lock();
		while ( m_nNumItems )
			Delete(0);

		delete [] m_ppData;
		m_ppData = NULL;
		m_nNumItems = 0;
		Unlock();
	}

	//////////////////////////////////////////////////////////////////

	virtual Type* Remove(int nIndex)
	{
		if ( nIndex < 0 || nIndex >= m_nNumItems )
			return false;

		Lock();
		Type*				pReturn = m_ppData[nIndex];
		Type**				ppNew = NULL;

		ppNew = new Type*[m_nNumItems-1];
		memset(ppNew, 0, sizeof(Type*) * (m_nNumItems-1));
		memcpy(ppNew, m_ppData, nIndex * sizeof(Type*));
		memcpy(&ppNew[nIndex], &m_ppData[nIndex+1], (m_nNumItems-nIndex-1) * sizeof(Type*));
		delete [] m_ppData;
		m_ppData = ppNew;
		m_nNumItems--;
		Unlock();

		return pReturn;	
	}

	//////////////////////////////////////////////////////////////////
	
	virtual void RemoveAll()
	{
		Lock();
		delete [] m_ppData;
		m_ppData = NULL;
		m_nNumItems = 0;
		Unlock();
	}

	//////////////////////////////////////////////////////////////////

	virtual Type* GetAt(int nIndex)
	{
		if ( nIndex < 0 || nIndex >= m_nNumItems )
			return false;

		return (m_ppData[nIndex]);
	}

	//////////////////////////////////////////////////////////////////

	virtual void SetAt(int nIndex, Type* pData)
	{
		if ( nIndex < 0 || nIndex >= m_nNumItems )
			return;

		Lock();
		m_ppData[nIndex] = pData;
		Unlock();
	}

	//////////////////////////////////////////////////////////////////

	virtual void Move(int nFrom, int nTo)
	{
		if ( nFrom < 0 || nFrom >= m_nNumItems || nTo < 0 || nTo >= m_nNumItems )
			return;

		Lock();
		Type*				pTemp = NULL;

		pTemp = m_ppData[nFrom];
		m_ppData[nFrom] = m_ppData[nTo];
		m_ppData[nTo] = pTemp;
		Unlock();
	}

	//////////////////////////////////////////////////////////////////

	void Lock()
	{
		EnterCriticalSection(&m_csLock);
	}

	//////////////////////////////////////////////////////////////////

	void Unlock()
	{
		LeaveCriticalSection(&m_csLock);
	}

	//////////////////////////////////////////////////////////////////


	// getters
	inline int						GetNumItems		( )						{ return m_nNumItems; }

	// wrappers
	inline int						GetSize			( )						{ return ( GetNumItems() ); }
	virtual Type*					RemoveAt		( int nIndex )			{ return ( Remove ( nIndex ) ); }
	virtual bool					DeleteAt		( int nIndex )			{ return ( Delete ( nIndex ) ); }

protected:
	int								m_nNumItems;
	Type**							m_ppData;

#ifdef _WIN32
	CRITICAL_SECTION				m_csLock;
#endif
};


#endif//_DATAMGR_H_
