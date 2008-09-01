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
#ifndef _CMDLINEINFO_H_
#define _CMDLINEINFO_H_

class CCmdLineInfo  
{
public:
	//
	// constructor/destructor
	CCmdLineInfo(int argc, char** argv, const char* lpszSwitches = "/-");
	virtual ~CCmdLineInfo();

	//
	// Methods
	const char*	GetArg(int nIndex);
	void			  SetSwitches(const char* lpszSwitches);
	bool			  IsSwitch(int nIndex);
	bool			  IsSwitchPresent(const char* lpszSwitch);
	int				  GetSwitchIndex(const char* lpszSwitch);
	int				  GetSwitchCount();
	int				  GetArgCount()								{ return m_nArgCount; }

protected:

	//
	// Attributes
	int				  m_nArgCount;
	char**			m_ppArgs;
	const char*	m_lpszSwitches;
};

#endif//_CMDLINEINFO_H_
