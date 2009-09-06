#ifndef _SWLOGICBASE_H_
#define _SWLOGICBASE_H_

#include <string>
#include "../common/game_objects/InGameIPAddress.h"
#include "../common/game_objects/NetworkPorts.h"
#include "../common/interfaces/ISoftwareLogic.h"
#include "WorldManager.h"

class SWLogicBase : public ISoftwareLogic
{
public:
  SWLogicBase(void);
  virtual ~SWLogicBase(void);

protected:

  bool isLocalIP(InGameIPAddress ip);
  bool serviceExists(const WorldManager::NetConnection& dest, FCULONG serviceType);
  bool getNetworkPortInfo(const WorldManager::NetConnection& dest, NetworkPorts& ports);
  std::vector<std::string> getArguments(const std::string& args);
};

#endif//_SWLOGICBASE_H_