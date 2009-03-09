#ifndef _NETWORKPORTS_H_
#define _NETWORKPORTS_H_

#include <vector>
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
  FCSHORT enablePort(FCSHORT portNum);
  FCSHORT installPort(FCSHORT portNum, FCULONG softwareType, FCULONG itemID);
  FCSHORT uninstallPort(FCSHORT portNum);
  FCSHORT getPortHealth(FCSHORT portNum);
  FCSHORT setPortHealth(FCSHORT portNum, FCSHORT health);
  // TODO: Need to add firewalls here as well

private:

  struct NetworkPort
  {
    bool enabled;
    FCSHORT portNum;
    FCULONG softwareType;
    FCULONG itemID;
    FCSHORT portHealth;
  };

  FCSHORT m_maxPort;
  typedef std::vector<NetworkPort> NetworkPortVector;
	NetworkPortVector m_ports;
};

#endif//_NETWORKPORTS_H_