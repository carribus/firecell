#include <sstream>
#include <vector>
#include "PlayerManager.h"
#include "WorldManager.h"
#include "SWLogicBase.h"

SWLogicBase::SWLogicBase(void)
{
}

///////////////////////////////////////////////////////////////////////

SWLogicBase::~SWLogicBase(void)
{
}

///////////////////////////////////////////////////////////////////////

bool SWLogicBase::isLocalIP(InGameIPAddress ip)
{
  bool bResult = false;

  if ( ip.GetClassA() == 127 && ip.GetClassB() == 0 && ip.GetClassC() == 0 && ip.GetClassD() == 1 )
    bResult = true;

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool SWLogicBase::serviceExists(const WorldManager::NetConnection& dest, FCULONG serviceType)
{
  NetworkPorts ports;
  bool bResult = false;
  
  if ( getNetworkPortInfo(dest, ports) )
  {
    bResult = ports.isServiceRunning(serviceType);
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

bool SWLogicBase::getNetworkPortInfo(const WorldManager::NetConnection& dest, NetworkPorts& ports)
{
  bool bResult = false;

  switch ( dest.connType )
  {
  case  WorldManager::NetConnection::Company:
    {
      Company* pCompany = WorldManager::instance().GetCompanyByIP( const_cast<InGameIPAddress&>(dest.ipAddress) );

      if ( pCompany )
      {
        ports = pCompany->GetComputer().GetNetworkPorts();
        bResult = true;
      }
    }
    break;

  case  WorldManager::NetConnection::Player:
    {
      Player* pPlayer = PlayerManager::instance().GetPlayerByID(dest.objID);

      if ( pPlayer )
      {
        ports = pPlayer->GetComputer().GetNetworkPorts();
        bResult = true;
      }
    }
    break;

  case  WorldManager::NetConnection::NPC:
    {
    }
    break;

  default:
    break;
  }

  return bResult;
}

///////////////////////////////////////////////////////////////////////

std::vector<std::string> SWLogicBase::getArguments(const std::string& args)
{
  std::istringstream iss(args);
  std::string arg;
  std::vector<std::string> arguments;

  while ( iss >> arg )
    arguments.push_back(arg);

  return arguments;
}