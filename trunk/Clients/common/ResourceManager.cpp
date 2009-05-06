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
#include "ResourceManager.h"

ResourceManager* ResourceManager::m_pThis = NULL;

ResourceManager::ResourceManager(void)
{
}

///////////////////////////////////////////////////////////////////////

ResourceManager::~ResourceManager(void)
{
}

///////////////////////////////////////////////////////////////////////

ResourceManager& ResourceManager::instance()
{
  if ( !m_pThis )
  {
    m_pThis = new ResourceManager;
  }

  return *m_pThis;
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::destroy()
{
  if ( m_pThis )
  {
    delete m_pThis;
    m_pThis = NULL;
  }
}

///////////////////////////////////////////////////////////////////////

int ResourceManager::LoadClientStrings(string string_file)
{
	if ( string_file.empty() )
		return -1;

	IrrXMLReaderUTF16* pXML = createIrrXMLReaderUTF16(string_file.c_str());

	if ( pXML )
	{
		ParseClientStrings(pXML);
	}

	return (int)m_mapStrings.size();
}

///////////////////////////////////////////////////////////////////////

int ResourceManager::LoadMissionStrings(string mission_file)
{
  if ( mission_file.empty() )
    return -1;

  IrrXMLReader* pXML = createIrrXMLReader(mission_file.c_str());

  if ( pXML)
  {
    ParseMissionStrings(pXML);
  }

  return (int)m_mapMissions.size();
}

///////////////////////////////////////////////////////////////////////

wstring ResourceManager::GetClientString(const wstring& key)
{
	std::wstring val;
	StringMap::iterator it = m_mapStrings.find(key);

	if ( it != m_mapStrings.end() )
		val = it->second;
	else
		val = L"<STRING [" + key + L"] NOT FOUND>";

	return val;
}

///////////////////////////////////////////////////////////////////////

std::string ResourceManager::GetMissionString(FCULONG mission_id, E_MISSIONSTRING_TYPE type)
{
	MissionMap::iterator it = m_mapMissions.find(mission_id);
	string val;

	if ( it == m_mapMissions.end() )
		return val;

	switch ( type )
	{
	case	MS_Name:
		val = it->second.name;
		break;

	case	MS_Prelude:
		val = it->second.prelude;
		break;

	case	MS_Desc:
		val = it->second.description;
		break;

	default:
		break;
	}

	return val;
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::ParseClientStrings(IrrXMLReaderUTF16* pXML)
{
	if ( !pXML )
		return;

	wstring elemName, stringID, stringText;

	while ( pXML->read() )
	{
		switch ( pXML->getNodeType() )
		{
		case	EXN_ELEMENT:
			{
				elemName = (wchar_t*)pXML->getNodeName();
				if ( elemName == L"FCStrings" )
				{
				}
				else if ( elemName == L"String" )
				{
					// get the ID of the string
					stringID = (wchar_t*)pXML->getAttributeValue((char16*)L"id");
				}
			}
			break;

		case	EXN_TEXT:
			{
				if ( !stringID.empty() )
				{
					size_t pos = 0;
					stringText = (wchar_t*)pXML->getNodeData();
					while ( (pos = stringText.find( L"\\n" )) != wstring::npos )
					{
						stringText.replace(pos, 2, L"\n");
					}
					m_mapStrings[stringID] = stringText;

					stringID = stringText = L"";
				}
			}
			break;
		}
	}
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::ParseMissionStrings(IrrXMLReader* pXML)
{
  if ( !pXML )
    return;

  string elemName;

  while ( pXML->read() )
  {
    switch ( pXML->getNodeType() )
    {
    case  EXN_ELEMENT:
      {
        elemName = pXML->getNodeName();
        if ( elemName == "Missions" )
        {
        }
        else if ( elemName == "Mission" )
        {
          ParseMissionNode(pXML);
        }
      }
      break;

    case  EXN_ELEMENT_END:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::ParseMissionNode(IrrXMLReader* pXML)
{
  string elemName, attrName, value;
  string* pTargetField = NULL;
  MissionStrings ms;
  bool bWorking = true;

  if ( !pXML->isEmptyElement() )
  {
    // get the attributes of the mission
    for ( int i = 0; i < pXML->getAttributeCount(); i++ )
    {
      attrName = pXML->getAttributeName(i);

      if ( attrName == "id" )
      {
        ms.id = pXML->getAttributeValueAsInt(i);
      }
    }

    // recurse
    while ( bWorking && pXML->read() )
    {
      switch ( pXML->getNodeType() )
      {
      case  EXN_ELEMENT:
        {
          elemName = pXML->getNodeName();

          if ( elemName == "Name" )
          {
            pTargetField = &ms.name;
          }
          else if ( elemName == "Prelude" )
          {
            pTargetField = &ms.prelude;
          }
          else if ( elemName == "Description" )
          {
            pTargetField = &ms.description;
          }
          else
          {
            pTargetField = NULL;
          }
        }
        break;

      case  EXN_ELEMENT_END:
        {
          elemName = pXML->getNodeName();

          if ( elemName == "Mission" )
            bWorking = false;
          else
            pTargetField = NULL;
        }
        break;

      case  EXN_TEXT:
        {
          if ( pTargetField )
          {
            // fill the target field
            *pTargetField = pXML->getNodeData();
            // and immediately remove the target
            pTargetField = NULL;
          }
        }
        break;
      }
    }

    // add the object to the map
    m_mapMissions[ ms.id ] = ms;
  }
}