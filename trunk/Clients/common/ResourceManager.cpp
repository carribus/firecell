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

	IrrXMLReader* pXML = createIrrXMLReader(string_file.c_str());

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

void ResourceManager::ParseClientStrings(IrrXMLReader* pXML)
{
	if ( !pXML )
		return;

	string elemName, stringID, stringText;

	while ( pXML->read() )
	{
		switch ( pXML->getNodeType() )
		{
		case	EXN_ELEMENT:
			{
				elemName = pXML->getNodeName();
				if ( elemName == "FCStrings" )
				{
				}
				else if ( elemName == "String" )
				{
					// get the ID of the string
					stringID = pXML->getAttributeValue("id");
				}
			}
			break;

		case	EXN_TEXT:
			{
				if ( !stringID.empty() )
				{
					stringText = pXML->getNodeData();
					m_mapStrings[stringID] = stringText;

					stringID = stringText = "";
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