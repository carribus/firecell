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

FCSHORT NetworkPorts::addPort()
{
  NetworkPort np = { false, m_maxPort+1, 0, 0, 0 };

  m_maxPort++;
  m_ports.push_back(np);

  return m_maxPort-1;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::getPortCount()
{
	return m_maxPort;
}

////////////////////////////////////////////////////////////////////////

bool NetworkPorts::isPortEnabled(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return false;

  NetworkPort& port = m_ports.at(portNum);

  return port.enabled;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::enablePort(FCSHORT portNum, bool bEnable)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);
  FCSHORT result = NPE_OK;

  // check if the port has a service associated with it
  if ( bEnable )
  {
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
  }
  else
    port.enabled = bEnable;

  return result;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::installPort(FCSHORT portNum, FCULONG softwareType, FCULONG itemID)
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

FCSHORT NetworkPorts::uninstallPort(FCSHORT portNum)
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

FCSHORT NetworkPorts::getPortMaxHealth(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  return port.portMaxHealth;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::getPortHealth(FCSHORT portNum)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  return port.portHealth;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::setPortMaxHealth(FCSHORT portNum, FCSHORT health)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  port.portMaxHealth = health;

  return NPE_OK;
}


////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::setPortHealth(FCSHORT portNum, FCSHORT health)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  port.portHealth = health;

  return NPE_OK;
}

////////////////////////////////////////////////////////////////////////

FCSHORT NetworkPorts::getSoftwareInfo(FCSHORT portNum, FCULONG& itemID, FCULONG& softwareType)
{
  if ( portNum < 0 || portNum > m_maxPort )
    return NPE_PORT_DOESNT_EXIST;

  NetworkPort& port = m_ports.at(portNum);

  itemID = port.itemID;
  softwareType = port.softwareType;

  return NPE_OK;
}
