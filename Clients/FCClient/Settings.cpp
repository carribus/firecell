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
