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
#ifndef _INIFILE_H_
#define _INIFILE_H_

#include <string>
#include <map>
#include <list>

using namespace std;

//////////////////////////////////////////////////////////////
// Constants
//////////////////////////////////////////////////////////////
const char c_INISectionNameStart = '[';
const char c_INISectionNameEnd = ']';
const char c_INIKeyValueSep = '=';
const char c_INIComment = ';';

class INIFile
{
  typedef map< string, string >      ValuesMap;

	//////////////////////////////////////////////////////////////
	// class CINIFile::CSection
	//////////////////////////////////////////////////////////////
	class CSection
	{
	public:
		CSection();
		~CSection();

		size_t				AddKey(const string key, const string value = "");
		string    		GetKey(int nIndex);
    string			  GetValue(const string key);
		size_t		    GetCount()							    { return m_mapValues.size(); }
    void			    SetName(std::string name)		{ m_strName = name; }
    string	      GetName()							      { return m_strName; }

	private:
    string        m_strName;
    ValuesMap			m_mapValues;
  };

public:
  INIFile(void);
  virtual ~INIFile(void);

	int				  Load(const string filename);
	CSection*		GetSection(const string name);
	CSection*		GetSection(size_t nIndex);
	CSection*		AddSection(string name);
	size_t      GetKeyCount()						{ return m_lstSections.size(); }

  short       GetValueShort(const string section, const string key);

private:

	//////////////////////////////////////////////////////////////
	// PRIVATE: Methods
	//////////////////////////////////////////////////////////////
	bool			ParseFile(ifstream& f);

  typedef std::list< CSection* > SectionList;
  SectionList     m_lstSections;
};

#endif//_INIFILE_H_