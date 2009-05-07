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
#include <stdlib.h>
#include <string.h>
#include "fctypes.h"
#include "fcfuncs.h"
#include "CmdLineInfo.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdLineInfo::CCmdLineInfo(int argc, char** argv, const char* lpszSwitches)
: m_nArgCount(argc)
, m_ppArgs(NULL)
, m_lpszSwitches(NULL)
{
	size_t				nLen;

	// copy the arguments array
	m_ppArgs = new char*[argc];
	for ( int i = 0; i < argc; i++ )
	{
		nLen = strlen(argv[i]);
		m_ppArgs[i] = new char[ nLen+1 ];
		memset(m_ppArgs[i], 0, (nLen+1)*sizeof(char));
		memcpy(m_ppArgs[i], argv[i], nLen*sizeof(char));
	}

	SetSwitches(lpszSwitches);
}

//////////////////////////////////////////////////////////////////////

CCmdLineInfo::~CCmdLineInfo()
{
	if ( m_ppArgs )
	{
		for ( int i = 0; i < m_nArgCount; i++ )
			delete [] m_ppArgs[i];
		delete [] m_ppArgs;
	}

	if ( m_lpszSwitches )
    free((void*)m_lpszSwitches);

}

//////////////////////////////////////////////////////////////////////

const char* CCmdLineInfo::GetArg(int nIndex)
{
	if ( nIndex < 0 || nIndex >= m_nArgCount )
		return NULL;

	return m_ppArgs[nIndex];
}

//////////////////////////////////////////////////////////////////////

void CCmdLineInfo::SetSwitches(const char* lpszSwitches)
{
	if ( m_lpszSwitches )
	{
		delete [] m_lpszSwitches;
		m_lpszSwitches = NULL;
	}
	if ( !lpszSwitches )
		return;

	size_t				nLen;

	// assign the switches
	nLen = strlen(lpszSwitches);
#ifdef _WIN32
  m_lpszSwitches = _strdup(lpszSwitches);
#else
  m_lpszSwitches = strdup(lpszSwitches);
#endif//_WIN32
}

//////////////////////////////////////////////////////////////////////

bool CCmdLineInfo::IsSwitch(int nIndex)
{
	const char*		lpszArg = GetArg(nIndex);

	if ( !lpszArg )
		return false;

	const char*		pSwitch = m_lpszSwitches;

	while ( *pSwitch )
	{
		if ( *pSwitch++ == lpszArg[0] )
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////

bool CCmdLineInfo::IsSwitchPresent(const char* lpszSwitch)
{
	if ( !lpszSwitch )
		return false;

	char*			pArg = NULL;
	for ( int i = 0; i < m_nArgCount; i++ )
	{
		if ( IsSwitch(i) )
		{
			pArg = m_ppArgs[i];
			if ( !fcstrcmpi( &pArg[1], lpszSwitch) )
				return true;
		}
	}

	return false;
}

//////////////////////////////////////////////////////////////////////

int CCmdLineInfo::GetSwitchIndex(const char* lpszSwitch)
{
	if ( !lpszSwitch )
		return -1;

	char*			pArg = NULL;
	for ( int i = 0; i < m_nArgCount; i++ )
	{
		if ( IsSwitch(i) )
		{
			pArg = m_ppArgs[i];
			if ( !fcstrcmpi( &pArg[1], lpszSwitch) )
				return i;
		}
	}

	return -1;
}

//////////////////////////////////////////////////////////////////////

int CCmdLineInfo::GetSwitchCount()
{
	if ( !m_ppArgs )
		return 0;

	int			nCount = 0;

	for ( int i = 0; i < m_nArgCount; i++ )
	{
		if ( IsSwitch(i) )
			nCount++;
	}

	return nCount;
}
