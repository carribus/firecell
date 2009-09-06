#include <sstream>
#include <vector>
#include "../common/game_objects/InGameIPAddress.h"
#include "../common/game_objects/NetworkPorts.h"
#include "PlayerManager.h"
#include "WorldManager.h"
#include "SWPortScan.h"

SWPortScan::SWPortScan(void)
{
}

///////////////////////////////////////////////////////////////////////

SWPortScan::~SWPortScan(void)
{
}

///////////////////////////////////////////////////////////////////////

bool SWPortScan::Execute(Player* pPlayer, const std::string& cmd, const std::string& args, std::string& result)
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

    if ( isLocalIP(ip) )
    {
      ip = pPlayer->GetIP();
    }

    if ( worldMgr.GetNetworkObject(ip, dest) )
    {
      switch ( dest.connType )
      {
      case  WorldManager::NetConnection::Company:
        {
          Company* pCompany = worldMgr.GetCompanyByIP(ip);

          ss << "portscan for " << ipAddress.c_str() << "\n\n";
          result = getPortDetails(pCompany);

          result.insert(0, ss.str());
        }
        break;

      case  WorldManager::NetConnection::Player:
        {
          PlayerManager& playerMgr = PlayerManager::instance();
          Player* pPlayer = playerMgr.GetPlayerByID(dest.objID);

          if ( pPlayer )
          {
            ss << "portscan for " << ipAddress.c_str() << "\n\n";
            result = getPortDetails(pPlayer);
            result.insert(0, ss.str());
          }
        }
        break;
       
      case  WorldManager::NetConnection::NPC:
        break;
      }
    }
    else
    {
      result = "portscan: IP Address ";
      result += ipAddress;
      result += " unreachable.\n\n";
    }
  }
  else
  {
    result = "Usage:  portscan [ip address]\n\nExample: portscan 127.0.0.1\n\n";
  }

  return true;
}

///////////////////////////////////////////////////////////////////////

std::string SWPortScan::getPortDetails(Company* pCompany)
{
  std::string result;

  if ( pCompany )
  {
    std::stringstream ss;
    pCompany->ReadLock();
    NetworkPorts& ports = pCompany->GetComputer().GetNetworkPorts();

    FCSHORT portCount = ports.getPortCount();
    FCULONG itemID, softwareType;

    if ( portCount )
    {
      for ( FCSHORT port = 0; port < portCount; port++ )
      {
        if ( ports.isPortEnabled(port) )
        {
          ss << "[" << port+1 << "] ";
          ports.getSoftwareInfo(port, itemID, softwareType);
          switch ( softwareType )
          {
          case  SWT_HTTPSERVER:
            ss << "Web Service\n";
            break;

          case  SWT_FTPSERVER:
            ss << "FTP Service\n";
            break;

          case  SWT_MAILSERVER:
            ss << "Mail Service\n";
            break;

          case  SWT_BANKSERVER:
            ss << "Bank Service\n";
            break;

          case  SWT_DBSERVER:
            ss << "Database Service\n";
            break;

          case  SWT_SSHSERVER:
            ss << "Secure Shell Service\n";
            break;
          }
        }
      }
    }
    else
    {
      ss << "No open ports detected\n";
    }
    ss << "\n";
    pCompany->Unlock();
    result = ss.str();
  }

  return result;
}

///////////////////////////////////////////////////////////////////////

std::string SWPortScan::getPortDetails(Player* pPlayer)
{
  std::string result;

  if ( pPlayer )
  {
    std::stringstream ss;
    pPlayer->ReadLock();
    NetworkPorts& ports = pPlayer->GetComputer().GetNetworkPorts();

    FCSHORT portCount = ports.getPortCount();
    FCULONG itemID, softwareType;

    if ( portCount )
    {
      for ( FCSHORT port = 0; port < portCount; port++ )
      {
        if ( ports.isPortEnabled(port) )
        {
          ss << "[" << port+1 << "] ";
          ports.getSoftwareInfo(port, itemID, softwareType);
          switch ( softwareType )
          {
          case  SWT_HTTPSERVER:
            ss << "Web Service\n";
            break;

          case  SWT_FTPSERVER:
            ss << "FTP Service\n";
            break;

          case  SWT_MAILSERVER:
            ss << "Mail Service\n";
            break;

          case  SWT_BANKSERVER:
            ss << "Bank Service\n";
            break;

          case  SWT_DBSERVER:
            ss << "Database Service\n";
            break;

          case  SWT_SSHSERVER:
            ss << "Secure Shell Service\n";
            break;
          }
        }
      }
    }
    else
    {
      ss << "No open ports detected\n";
    }
    ss << "\n";
    pPlayer->Unlock();
    result = ss.str();
  }

  return result;
}

