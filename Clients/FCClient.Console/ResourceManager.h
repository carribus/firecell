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

  int LoadMissionStrings(string mission_file);

private:

  void ParseMissionStrings(IrrXMLReader* pXML);
    void ParseMissionNode(IrrXMLReader* pXML);

  static ResourceManager*          m_pThis;

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