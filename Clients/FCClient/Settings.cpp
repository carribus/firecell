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
#include "Settings.h"

Settings* Settings::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

Settings::Settings(void)
{
}

///////////////////////////////////////////////////////////////////////

Settings::~Settings(void)
{
}

///////////////////////////////////////////////////////////////////////

Settings& Settings::instance()
{
	if ( !m_pThis )
	{
		m_pThis = new Settings;
	}

	return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void Settings::destroy()
{
	if ( m_pThis )
	{
		delete m_pThis;
		m_pThis = NULL;
	}
}

///////////////////////////////////////////////////////////////////////

bool Settings::LoadSettings(string filename)
{
	if ( filename.empty() )
		return false;

	std::string elemName, currentPath, key, value, attrName;
	IrrXMLReader* pXML = createIrrXMLReader(filename.c_str());

	if ( pXML )
	{
		while ( pXML->read() )
		{
			switch ( pXML->getNodeType() )
			{
			case	EXN_ELEMENT:
				{
					elemName = pXML->getNodeName();
					if ( currentPath.length() )
						currentPath += "/";
					currentPath += elemName;

					unsigned int attrCount = pXML->getAttributeCount();

					for ( unsigned int i = 0; i < attrCount; i++ )
					{
						attrName = pXML->getAttributeName(i);
						value = pXML->getAttributeValue(i);

						key = currentPath + ":" + attrName;
						m_mapKeyToValue[ key ] = value;
					}

					if ( pXML->isEmptyElement() )
					{
						size_t pos = currentPath.rfind( elemName );
						currentPath.erase( (pos > 0 ? pos-1 : pos) , currentPath.length() );
					}
				}
				break;

			case	EXN_ELEMENT_END:
				{
					elemName = pXML->getNodeName();
					size_t pos = currentPath.rfind( elemName );
					currentPath.erase( (pos > 0 ? pos-1 : pos) , currentPath.length() );
				}
				break;
			}
		}
	}

	return true;
}

///////////////////////////////////////////////////////////////////////

string Settings::GetValue(const string& path, const string& attrib)
{
	string ret = "";
	string key = path + ":" + attrib;
	std::map<string, string>::iterator it = m_mapKeyToValue.find(key);

	if ( it != m_mapKeyToValue.end() )
		ret = it->second;

	return ret;
}
