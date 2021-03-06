/*
    FireCell Qt Client - The Qt client code for the firecell multiplayer game
    Copyright (C) 2008-2010  Peter M. Mares

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
#include "StdAfx.h"
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

#include <QDomDocument>

bool Settings::LoadSettings(QString filename)
{
	if ( filename.isEmpty() )
		return false;

  QFile file(filename);

  if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    return false;

  QXmlStreamReader xml;
  QString currentPath, key, value, attrName;

  xml.setDevice(&file);
  while ( !xml.atEnd() ) //&& !xml.hasError() )
  {
    switch ( xml.readNext() )
    {
    case  QXmlStreamReader::StartElement:
      {
        if ( currentPath.length() )
          currentPath += "/";
        currentPath += xml.name();

        QXmlStreamAttributes attribs = xml.attributes();

        unsigned int attrCount = attribs.size();

        for ( unsigned int i = 0; i < attrCount; i++ )
        {
	        attrName = attribs[i].name().toString();
	        value = attribs[i].value().toString();

	        key = currentPath + ":" + attrName;
	        m_mapKeyToValue[ key ] = value;
        }
      }
      break;

    case  QXmlStreamReader::EndElement:
      {
        size_t pos = currentPath.lastIndexOf( xml.name().toString() );
        currentPath.remove( (pos > 0 ? pos-1 : pos) , currentPath.length() );
      }
      break;

    default:
      break;
    }
  }
  if ( xml.hasError() )
  {
    QMessageBox::critical(NULL, 
                        "Settings", 
                        xml.errorString(), 
                        QMessageBox::Ok);
  }

  file.close();

  return true;
}

///////////////////////////////////////////////////////////////////////

QString Settings::GetValue(const QString& path, const QString& attrib)
{
	QString ret = "";
	QString key = path + ":" + attrib;
  QMap<QString, QString>::iterator it = m_mapKeyToValue.find(key);

	if ( it != m_mapKeyToValue.end() )
		ret = it.value();

	return ret;
}
