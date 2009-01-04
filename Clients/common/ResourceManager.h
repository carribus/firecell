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

	int LoadClientStrings(string string_file);
  int LoadMissionStrings(string mission_file);

private:

	void ParseClientStrings(IrrXMLReader* pXML);
  void ParseMissionStrings(IrrXMLReader* pXML);
    void ParseMissionNode(IrrXMLReader* pXML);

  static ResourceManager*          m_pThis;

	/*
	 *	Client string related objects
	 */
	typedef map<string, string> StringMap;
	StringMap m_mapStrings;

  /*
   *  Mission related objects
   */
  struct MissionStrings
  {
    FCULONG id;
    string name;
    string prelude;
    string description;
  };
  typedef map<FCULONG, MissionStrings> MissionMap;
  MissionMap m_mapMissions;
};

#endif//_RESOURCEMANAGER_H_