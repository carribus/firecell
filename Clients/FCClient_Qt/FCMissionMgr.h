#ifndef _FCMISSIONMGR_H_
#define _FCMISSIONMGR_H_

#include "../../common/fctypes.h"
#include <list>

class FCMissionMgr
{
public:
  FCMissionMgr(void);
  ~FCMissionMgr(void);

  bool addMission(FCULONG mission_id, FCSHORT successCount = 0, FCSHORT failureCount = 0, bool bCompleted = false, FCULONG parent_mission_id = 0);
	bool completeMission(FCULONG mission_id);
	bool removeMission(FCULONG mission_id);

	bool isMissionAccepted(FCULONG mission_id);
	bool isMissionComplete(FCULONG mission_id);
  FCSHORT getMissionSuccessCount(FCULONG mission_id);
  FCSHORT getMissionFailureCount(FCULONG mission_id);

private:

  struct _Mission
  {
    FCULONG mission_id;
    FCSHORT successCount;
    FCSHORT failureCount;
    bool bCompleted;
    std::list<_Mission> children;
  };

  _Mission* getMission(FCULONG mission_id);

  std::list<_Mission> m_missions;
};

#endif//_FCMISSIONMGR_H_