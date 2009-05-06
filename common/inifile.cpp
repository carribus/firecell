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
#include <algorithm>
#include <string>
#include <cctype>
#include <fstream>
#include "inifile.h"

INIFile::CSection::CSection()
{
}

///////////////////////////////////////////////////////////////////////

INIFile::CSection::~CSection()
{
  m_mapValues.clear();
}

///////////////////////////////////////////////////////////////////////

size_t INIFile::CSection::AddKey(const string key, const string value)
{
  ValuesMap::iterator   it = m_mapValues.find(key);

  if ( it == m_mapValues.end() )
  {
    m_mapValues[key] = value;
    return m_mapValues.size();
  }

  return -1;
}

///////////////////////////////////////////////////////////////////////

string INIFile::CSection::GetKey(int nIndex)
{
  if ( nIndex < 0 || nIndex >= (int)m_mapValues.size()-1 )
    return "";

  string strResult;
  int nPos = 0;
  for ( ValuesMap::iterator it = m_mapValues.begin(); it != m_mapValues.end(); it++ )
  {
    if ( nPos == nIndex )
    {
      strResult = it->first;
      break;
    }
  }

  return strResult;
}

///////////////////////////////////////////////////////////////////////

string INIFile::CSection::GetValue(const string key)
{
  string strVal, strKey = key;

  std::transform(strKey.begin(), strKey.end(), strKey.begin(), (int(*)(int))tolower);
  strVal = m_mapValues[strKey];

  return strVal;
}

///////////////////////////////////////////////////////////////////////

INIFile::INIFile(void)
{
}

///////////////////////////////////////////////////////////////////////

INIFile::~INIFile(void)
{
	SectionList::iterator it = m_lstSections.begin();
	SectionList::iterator limit = m_lstSections.end();

	for ( ; it != limit; it++ )
	{
		delete *it;
	}
}

///////////////////////////////////////////////////////////////////////

int INIFile::Load(const string filename)
{
  if ( filename.empty() )
    return -1;

  ifstream    f;

  f.open(filename.c_str());
  if ( f.is_open() )
  {
    ParseFile(f);
  }
  else
    return -1;

  f.close();
  
  return 0;
}

///////////////////////////////////////////////////////////////////////

INIFile::CSection* INIFile::GetSection(const string name)
{
  string n = name;

  std::transform(n.begin(), n.end(), n.begin(), (int(*)(int))tolower);
  for ( SectionList::iterator it = m_lstSections.begin(); it != m_lstSections.end(); it++ )
  {
    if ( n.compare( (*it)->GetName() ) == 0 )
      return *it;
  }

  return NULL;
}

///////////////////////////////////////////////////////////////////////

INIFile::CSection* INIFile::GetSection(size_t nIndex)
{
  if ( nIndex < 0 || nIndex >= m_lstSections.size()-1 )
    return NULL;

  CSection* pSection = NULL;
  int nPos = 0;
  for ( SectionList::iterator it = m_lstSections.begin(); it != m_lstSections.end(); it++ )
  {
    if ( nPos == nIndex )
    {
      pSection = *it;
      break;
    }
  }

  return pSection;
}

///////////////////////////////////////////////////////////////////////

INIFile::CSection* INIFile::AddSection(string name)
{
	if ( !name.empty() )
		return NULL;

	CSection*		pSection = new CSection;

	pSection->SetName(name);
	m_lstSections.push_back(pSection);
	return pSection;
}

///////////////////////////////////////////////////////////////////////

string INIFile::GetValueString(const string section, const string key)
{
  CSection* pSection = GetSection(section);
  string val;

  if ( pSection )
  {
    val = pSection->GetValue(key);
  }

  return val;
}

///////////////////////////////////////////////////////////////////////

short INIFile::GetValueShort(const string section, const string key)
{
  CSection* pSection = GetSection(section);
  short sRet = 0;
  string val;

  if ( pSection )
  {
    val = pSection->GetValue(key);
    sRet = (short)atoi(val.c_str());
  }

  return sRet;
}

///////////////////////////////////////////////////////////////////////

bool INIFile::ParseFile(ifstream& f)
{
  if ( !f.is_open() )
    return false;

  char* pPos1 = NULL, *pPos2 = NULL;
  int nLine = 0;
  char buffer[1024];
  string strSection, strKey, strVal;
  CSection* pSection = NULL;

  while ( f.getline(buffer, sizeof(buffer)) )
  {
		nLine++;

		// is this the beginning of a section?
		if ( buffer[0] == c_INISectionNameStart )
		{
			// yes, so get the end of the section name
			pPos1 = strchr( buffer, c_INISectionNameEnd );
			*pPos1 = '\0';
			pSection = new CSection;
			m_lstSections.push_back(pSection);
      strSection = &buffer[1];
      std::transform(strSection.begin(), strSection.end(), strSection.begin(), (int(*)(int))tolower);
			pSection->SetName( strSection );
			continue;
		}
		// check that it is not a comment
		if ( buffer[0] != c_INIComment )
		{
			if ( (pPos2 = strchr( buffer, '=' )) )
			{
				*pPos2 = '\0';
				strKey = buffer;
        // make the key lower case
        std::transform(strKey.begin(), strKey.end(), strKey.begin(), (int(*)(int))tolower);
        // trim the key
				strKey.erase( strKey.find_last_not_of(" \r\n")+1);
				strVal = pPos2+1;
        // trim the value
				strKey.erase( strKey.find_last_not_of(" \r\n")+1);
				if ( pSection )
				{
					pSection->AddKey( strKey, strVal);
				}
				else
				{
          return false;
				}
			}
		}
  }

  return true;
}
