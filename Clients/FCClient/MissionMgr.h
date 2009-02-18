#ifndef _MISSIONMGR_H_
#define _MISSIONMGR_H_

#include "../../common/fctypes.h"
#include <list>

class MissionMgr
{
public:
  MissionMgr(void);
  ~MissionMgr(void);

  bool addMission(FCULONG mission_id, bool bCompleted = false, FCULONG parent_mission_id = 0);

private:


  struct _Mission
  {
    FCULONG mission_id;
    bool bCompleted;
    std::list<_Mission> children;
  };

  _Mission* getMission(FCULONG mission_id);

  std::list<_Mission> m_missions;
};

#endif//_MISSIONMGR_H_