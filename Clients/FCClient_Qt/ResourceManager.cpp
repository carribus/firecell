#include "StdAfx.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::m_pThis = NULL;

///////////////////////////////////////////////////////////////////////

ResourceManager::ResourceManager()
{
}

///////////////////////////////////////////////////////////////////////

ResourceManager::~ResourceManager()
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

int ResourceManager::loadClientStrings(QString string_file)
{
	if ( string_file.isEmpty() )
		return -1;

  QFile file(string_file);

  if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    return false;

  QXmlStreamReader xml;
  xml.setDevice(&file);

  if ( !xml.atEnd() && !xml.hasError() )
  {
    parseClientStrings(xml);
  }

  return (int)m_mapStrings.size();
}

///////////////////////////////////////////////////////////////////////

int ResourceManager::loadMissionStrings(QString mission_file)
{
	if ( mission_file.isEmpty() )
		return -1;

  QFile file(mission_file);

  if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    return false;

  QXmlStreamReader xml;
  xml.setDevice(&file);

  if ( !xml.atEnd() && !xml.hasError() )
  {
    parseMissionStrings(xml);
  }

  return (int)m_mapStrings.size();
}

///////////////////////////////////////////////////////////////////////

QString ResourceManager::getClientString(const QString& key)
{
  QString val;
  StringMap::iterator it = m_mapStrings.find(key);

  if ( it != m_mapStrings.end() )
    val = it.value();
  else
    val = "<STRING [" + key + "] NOT FOUND>";

  return val;
}

///////////////////////////////////////////////////////////////////////

QString ResourceManager::getMissionString(FCULONG mission_id, E_MISSIONSTRING_TYPE type)
{
  MissionMap::iterator it = m_mapMissions.find(mission_id);
  QString val;

  if ( it == m_mapMissions.end() )
    return val;

  switch ( type )
  {
  case  MissionStringName:
    val = it.value().name;
    break;

  case  MissionStringPrelude:
    val = it.value().prelude;
    break;

  case  MissionStringDesc:
    val = it.value().description;
    break;

  default:
    break;
  }

  return val;
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::parseClientStrings(QXmlStreamReader& xml)
{
  if ( xml.atEnd() || xml.hasError() )
    return;

  QString elemName, stringID, stringText;

  while ( !xml.atEnd() )
  {
    switch ( xml.readNext() )
    {
    case  QXmlStreamReader::StartElement:
      {
        elemName = xml.name().toString();
        if ( elemName == "FCStrings" )
        {
        }
        else if ( elemName == "String" )
        {
          // get the ID of the string
          stringID = xml.attributes().value("id").toString();
          // get the actual string
          stringText = xml.readElementText();
          // parse the string for endline characters
          stringText.replace("\\n", "\n");
          m_mapStrings[stringID] = stringText;
        }
      }
      break;

    default:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::parseMissionStrings(QXmlStreamReader& xml)
{
  if ( xml.atEnd() || xml.hasError() )
    return;

  QString elemName, stringID, stringText;

  while ( !xml.atEnd() )
  {
    switch ( xml.readNext() )
    {
    case  QXmlStreamReader::StartElement:
      {
        elemName = xml.name().toString();
        if ( elemName == "Missions" )
        {
        }
        else if ( elemName == "Mission" )
        {
          parseMissionNode(xml);
        }
      }
      break;

    default:
      break;
    }
  }
}

///////////////////////////////////////////////////////////////////////

void ResourceManager::parseMissionNode(QXmlStreamReader& xml)
{
  QString elemName, attrName, value;
  MissionStrings ms;
  bool bWorking = true;

  if ( xml.isStartElement() )
  {
    QString mID = xml.attributes().value("id").toString();
    ms.id = mID.toULong();

    while ( bWorking && !xml.atEnd() )
    {
      switch ( xml.readNext() )
      {
      case  QXmlStreamReader::StartElement:
        {
          elemName = xml.name().toString();

          if ( elemName == "Name" )
          {
            ms.name = xml.readElementText();
          }
          else if ( elemName == "Prelude" )
          {
            ms.prelude = xml.readElementText();;
          }
          else if ( elemName == "Description" )
          {
            ms.description = xml.readElementText();;
          }
        }
        break;

      case  QXmlStreamReader::EndElement:
        {
          if ( xml.name() == "Mission" )
            bWorking = false;
        }
        break;

      default:
        break;
      }

    }

    m_mapMissions[ ms.id ] = ms;
  }
}
