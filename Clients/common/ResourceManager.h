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
#ifndef _RESOURCEMANAGER_H_
#define _RESOURCEMANAGER_H_

#include <map>
#include <string>
#include "../../common/fctypes.h"
#include "../../common/XML/irrXML.h"

using namespace irr;
using namespace io;

using namespace std;

class ResourceManager
{
  ResourceManager(void);
  ~ResourceManager(void);

public:

  static ResourceManager& instance();
  static void destroy();

  int LoadClientStrings(std::string string_file);
  int LoadMissionStrings(std::string mission_file);

  std::wstring GetClientString(const std::wstring& key);

private:

	void ParseClientStrings(IrrXMLReaderUTF16* pXML);
  void ParseMissionStrings(IrrXMLReader* pXML);
    void ParseMissionNode(IrrXMLReader* pXML);

  static ResourceManager*          m_pThis;

	/*
	 *	Client string related objects
	 */
  typedef std::map<std::wstring, std::wstring> StringMap;
	StringMap m_mapStrings;

  /*
   *  Mission related objects
   */
  struct MissionStrings
  {
    FCULONG id;
    std::string name;
    std::string prelude;
    std::string description;
  };
	typedef std::map<FCULONG, MissionStrings> MissionMap;
  MissionMap m_mapMissions;
};

#endif//_RESOURCEMANAGER_H_