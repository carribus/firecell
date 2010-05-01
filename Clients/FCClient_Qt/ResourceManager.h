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
#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <QMap>
#include <QString>
#include <QXmlStreamReader>
#include "../../common/fctypes.h"

class ResourceManager
{
  ResourceManager();
  ~ResourceManager();

public:

	enum E_MISSIONSTRING_TYPE
	{
		MissionStringName,
		MissionStringPrelude,
		MissionStringDesc
	};

  static ResourceManager& instance();
  static void destroy();

  int loadClientStrings(QString string_file);
  int loadMissionStrings(QString mission_file);

  QString getClientString(const QString& key);
	QString getMissionString(FCULONG mission_id, E_MISSIONSTRING_TYPE type);

private:

  void parseClientStrings(QXmlStreamReader& xml);
  void parseMissionStrings(QXmlStreamReader& xml);
    void parseMissionNode(QXmlStreamReader& xml);

  static ResourceManager*          m_pThis;

	/*
	 *	Client string related objects
	 */
  typedef QMap<QString, QString> StringMap;
	StringMap m_mapStrings;

  /*
   *  Mission related objects
   */
  struct MissionStrings
  {
    FCULONG id;
    QString name;
    QString prelude;
    QString description;
  };
	typedef QMap<FCULONG, MissionStrings> MissionMap;
  MissionMap m_mapMissions;
};

#endif // _RESOURCEMANAGER_H
