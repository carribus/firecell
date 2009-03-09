#include "NetworkPorts.h"

NetworkPorts::NetworkPorts(void)
: m_maxPort(0)
{
}

////////////////////////////////////////////////////////////////////////

NetworkPorts::~NetworkPorts(void)
{
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::AddPort()
{
  NetworkPort np = { false, m_maxPort+1, 0, 0, 0 };

  m_maxPort++;
  m_ports.push_back(np);

  return m_maxPort;
}

////////////////////////////////////////////////////////////////////////

bool NetworkPorts::IsPortEnabled(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return false;

  NetworkPort& port = m_ports.at(portNum);

  return port.enabled;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::EnablePort(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);
  FCSHORT result = NPE_OK;

  // check if the port has a service associated with it
  if ( port.softwareType )
  {
    if ( port.itemID )
    {
      port.enabled = true;
    }
    else
    {
      result = NPE_NO_SERVICE_ASSIGNED;
    }
  }
  else
  {
    result = NPE_NO_SOFTWARETYPE;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::InstallPort(FCSHORT portNum, FCULONG softwareType, FCULONG itemID)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);
  FCSHORT result = NPE_OK;
  
  if ( !port.softwareType )
  {
    if ( !port.itemID )
    {
      port.softwareType = softwareType;
      port.itemID = itemID;
    }
    else
    {
      result = NPE_SERVICE_ALREADY_ASSIGNED;
    }
  }
  else
  {
    result = NPE_SOFTWARETYPE_ALREADY_ASSIGNED;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::UninstallPort(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);
  FCSHORT result = NPE_OK;
  
  port.enabled = false;
  port.itemID = 0;
  port.softwareType = SWT_NONE;

  return result;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::GetPortHealth(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  return port.portHealth;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::SetPortHealth(FCSHORT portNum, FCSHORT health)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  port.portHealth = health;

  return NPE_OK;
}
