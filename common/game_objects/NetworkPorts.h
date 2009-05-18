#ifndef _NETWORKPORTS_H_
#define _NETWORKPORTS_H_

#include <vector>
#include "../PThreadRWLock.h"
#include "../fctypes.h"
#include "swtypes.h"

#define NPE_OK                              0
#define NPE_PORT_DOESNT_EXIST               -1
#define NPE_NO_SERVICE_ASSIGNED             -2
#define NPE_NO_SOFTWARETYPE                 -3
#define NPE_SERVICE_ALREADY_ASSIGNED        -4
#define NPE_SOFTWARETYPE_ALREADY_ASSIGNED   -5

class NetworkPorts
{
public:
	NetworkPorts(void);
	~NetworkPorts(void);

  FCSHORT addPort();
	FCSHORT getPortCount();
  bool isPortEnabled(FCSHORT portNum);
  FCSHORT enablePort(FCSHORT portNum, bool bEnable = true);
  FCSHORT installPort(FCSHORT portNum, FCULONG softwareType, FCULONG itemID);
  FCSHORT uninstallPort(FCSHORT portNum);
  FCSHORT getPortMaxHealth(FCSHORT portNum);
  FCSHORT getPortHealth(FCSHORT portNum);
  FCSHORT setPortMaxHealth(FCSHORT portNum, FCSHORT health);
  FCSHORT setPortHealth(FCSHORT portNum, FCSHORT health);
  FCSHORT getSoftwareInfo(FCSHORT portNum, FCULONG& itemID, FCULONG& softwareType);
  // TODO: Need to add firewalls here as well

  void lockForRead()                            { m_lock.LockForRead(); }
  void lockForWrite()                           { m_lock.LockForWrite(); }
  void unlock()                                 { m_lock.Unlock(); }

private:

  struct NetworkPort
  {
    bool enabled;
    FCSHORT portNum;
    FCULONG softwareType;
    FCULONG itemID;
    FCSHORT portMaxHealth;
    FCSHORT portHealth;
  };

  FCSHORT m_maxPort;
  typedef std::vector<NetworkPort> NetworkPortVector;
	NetworkPortVector m_ports;
  PThreadRWLock m_lock;
};

#endif//_NETWORKPORTS_H_