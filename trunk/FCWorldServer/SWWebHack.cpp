#include <sstream>
#include "../common/game_objects/NetworkPorts.h"
#include "PlayerManager.h"
#include "WorldManager.h"
#include "SWWebHack.h"

SWWebHack::SWWebHack(void)
{
}

///////////////////////////////////////////////////////////////////////

SWWebHack::~SWWebHack(void)
{
}

///////////////////////////////////////////////////////////////////////

bool SWWebHack::Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result)
{
  WorldManager& worldMgr = WorldManager::instance();
  std::vector<std::string>& arguments = getArguments(args);
  
  if ( arguments.size() >= 1 )
  {
    WorldManager::NetConnection dest;
    std::stringstream ss;
    std::string ipAddress = arguments.at(0);
    InGameIPAddress ip;

    ip.SetIP(ipAddress);

    // check if the player tried to attack themselves
    if ( isLocalIP(ip) )
    {
      ss << "WebHack error: You cannot attack yourself\n\n";
    }
    else
    {
      if ( worldMgr.GetNetworkObject(ip, dest) )
      {
        if ( serviceExists(dest, SWT_HTTPSERVER) )
        {
          // you left off here
        }
        else
        {
          ss << "WebHack error: No HTTP service available on target\n\n";
        }
      }
    }
  }

  return true;
}